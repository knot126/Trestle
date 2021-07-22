/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Simple JSON Parser
 */

#include <string.h>
#include <inttypes.h>

#include "alloc.h"
#include "str.h"
#include "log.h"
#include "fs.h"

#include "json.h"

typedef enum {
	DG_JSON_TOK_OPEN_BRACKET,
	DG_JSON_TOK_CLOSE_BRACKET,
	DG_JSON_TOK_OPEN_CURLY,
	DG_JSON_TOK_CLOSE_CURLY,
	DG_JSON_TOK_COMMA,
	DG_JSON_TOK_COLON,
	DG_JSON_TOK_STRING,
	DG_JSON_TOK_INTEGER,
	DG_JSON_TOK_NUMBER,
	DG_JSON_TOK_TRUE,
	DG_JSON_TOK_FALSE,
	DG_JSON_TOK_NULL,
} DgJSONTokenType;

static bool is_numeric(char c) {
	return ((c >= '0') && (c <= '9')) || (c == '.') || (c == '-');
}

static bool is_alpha(char c) {
	return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_');
}

static bool is_alphanumeric(char c) {
	return (is_alpha(c)) || ((c >= '0') && (c <= '9'));
}

typedef struct {
	DgJSONTokenType type;
	DgJSONValueData value;
	size_t offset;
} DgJSONToken;

static void DgJSONFreeToken(DgJSONToken *tok) {
	/**
	 * Free a token's memory if needed.
	 */
	
	if (tok->type == DG_JSON_TOK_STRING) {
		if (tok->value.as_string) {
			DgFree((void *) tok->value.as_string);
		}
	}
}

typedef struct {
	DgJSONToken *data;
	size_t count, alloc;
} DgJSONTokenArray;

static void DgJSONTokenArrayInit(DgJSONTokenArray * restrict array) {
	/**
	 * Initialise a token array.
	 */
	
	array->data = NULL;
	array->count = 0;
	array->alloc = 0;
}

static int32_t DgJSONPushToken(DgJSONTokenArray * restrict array, DgJSONToken token) {
	/**
	 * Push a token onto the array.
	 */
	
	if (array->count >= array->alloc) {
		array->alloc = (array->alloc > 0) ? array->alloc * 2 : 8;
		array->data = DgRealloc(array->data, sizeof *array->data * array->alloc);
		
		if (!array->data) {
			return 1;
		}
	}
	
	array->data[array->count] = token;
	array->count++;
	
	return 0;
}

static void DgJSONTokenArrayFree(DgJSONTokenArray * restrict array, bool failed) {
	/**
	 * Free a token array.
	 */
	
	if (array->data) {
		// In failmode, the strings won't ever be used again, so we need to free them.
		if (failed) {
			for (size_t i = 0; i < array->count; i++) {
				DgJSONFreeToken(&array->data[i]);
			}
		}
		
		DgFree(array->data);
	}
}

static int32_t DgJSONTokenise(DgJSONTokenArray * restrict tokens, const size_t length, const char * const restrict source) {
	/**
	 * Tokenise the given JSON data.
	 */
	
	DgJSONTokenArrayInit(tokens);
	
	for (size_t i = 0; i < length;) {
		int32_t status;
		
		if (source[i] == '{') {
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_OPEN_CURLY, 0, i++});
		}
		
		else if (source[i] == '}') {
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_CLOSE_CURLY, 0, i++});
		}
		
		else if (source[i] == '[') {
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_OPEN_BRACKET, 0, i++});
		}
		
		else if (source[i] == ']') {
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_CLOSE_BRACKET, 0, i++});
		}
		
		else if (source[i] == ',') {
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_COMMA, 0, i++});
		}
		
		else if (source[i] == ':') {
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_COLON, 0, i++});
		}
		
		// Handle strings, which run until the next quote mark.
		else if (source[i] == '"') {
			size_t start = i + 1;
			while (source[++i] != '"' && i < length);
			size_t end = i;
			
			char *s = DgStrdupl(&source[start], end - start);
			if (!s) {
				return 2;
			}
			
			DgLog(DG_LOG_VERBOSE, "%s", s);
			
			status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_STRING, .value.as_string = s, i++});
		}
		
		// Handle numbers, which are either integers or numbers (JSON does not 
		// limit us to just using double; to preserve percision, we allow 
		// storing integers as actual integers)
		else if (is_numeric(source[i])) {
			bool is_int = true;
			size_t start = i;
			while (is_numeric(source[i]) && i < length) {
				if (source[i++] == '.') {
					is_int = false;
				}
			}
			size_t end = i;
			
			char *s = DgStrdupl(&source[start], end - start);
			if (!s) {
				return 2;
			}
			
			DgLog(DG_LOG_VERBOSE, "%s", s);
			
			if (is_int) {
				status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_INTEGER, .value.as_integer = atoll(s), i});
			}
			else {
				status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_NUMBER, .value.as_number = strtod(s, NULL), i});
			}
			
			// Free temp string
			DgFree(s);
		}
		
		// Handle keywords. JSON only really has three: true, false and null.
		// HOWEVER, we do allow strings without quotes since we know what was
		// probably intended. This is not JSON but we will parse it anyways...
		else if (is_alpha(source[i])) {
			size_t start = i;
			while (is_alphanumeric(source[++i]) && i < length);
			size_t end = i;
			
			char *s = DgStrdupl(&source[start], end - start);
			if (!s) {
				return 2;
			}
			
			DgLog(DG_LOG_VERBOSE, "%s", s);
			
			// Accept null, true or false, otherwise treat as string
			if (!strcmp(s, "null")) {
				status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_NULL, 0, i});
				DgFree(s);
			}
			else if (!strcmp(s, "false")) {
				status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_FALSE, 0, i});
				DgFree(s);
			}
			else if (!strcmp(s, "true")) {
				status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_TRUE, 0, i});
				DgFree(s);
			}
			else {
				status = DgJSONPushToken(tokens, (DgJSONToken) {DG_JSON_TOK_STRING, .value.as_string = s, i});
			}
		}
		
		else if (source[i] == ' ' || source[i] == '\t' || source[i] == '\r' || source[i] == '\n') {
			i++;
		}
		
		else {
			DgLog(DG_LOG_ERROR, "JSON Tokeniser Error: Unrecognised char '%c'.", source[i++]);
			return 1;
		}
		
		if (status) {
			return status;
		}
	}
	
	return 0;
}

int32_t DgJSONParse(DgJSONValue * restrict doc, const size_t length, const char * const restrict source) {
	/**
	 * Parse a JSON document from the given buffer.
	 */
	
	DgJSONTokenArray tokens;
	
	if (DgJSONTokenise(&tokens, length, source)) {
		// Free token array, failure
		DgJSONTokenArrayFree(&tokens, true);
		return 1;
	}
	
	// Print tokens for debug
	for (size_t i = 0; i < tokens.count; i++) {
		if (tokens.data[i].type == DG_JSON_TOK_STRING) {
			DgLog(DG_LOG_VERBOSE, "(Token %d) Token = %d, Value = \"%s\"", i, tokens.data[i].type, tokens.data[i].value.as_string);
		}
		else if (tokens.data[i].type == DG_JSON_TOK_NUMBER) {
			DgLog(DG_LOG_VERBOSE, "(Token %d) Token = %d, Value = %g", i, tokens.data[i].type, tokens.data[i].value.as_number);
		}
		else {
			DgLog(DG_LOG_VERBOSE, "(Token %d) Token = %d, Value = %d", i, tokens.data[i].type, tokens.data[i].value);
		}
	}
	
	// Free token array, successful
	DgJSONTokenArrayFree(&tokens, false);
	
	return 0;
}
