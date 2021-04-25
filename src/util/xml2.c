/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A2
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "alloc.h"
#include "fs.h"

#include "xml2.h"

/**
 * Data Types
 */

typedef struct {
	enum {
		DG_XML_UNKNOWN,
		DG_XML_TAG_START,
		DG_XML_TAG_END,
		DG_XML_KEY,
		DG_XML_VALUE,
	} type;
	char *text;
} Dg_XMLParseToken;

/**
 * Utility Functions
 */

#define ALLOC_ERROR_CHECK(x) if (!x) { \
				printf("\033[1;33mWarning:\033[0m XML Parser Error: Failed to allocate memory for string.\n"); \
				return 1; \
			}

static Dg_XMLParseToken *token_list_expand(Dg_XMLParseToken *data, size_t * const current_size) {
	*current_size = *current_size + 1;
	return (Dg_XMLParseToken *) DgRealloc(data, *current_size * sizeof(Dg_XMLParseToken));
}

static bool is_whitespace(const char * const seq) {
	return (*seq == ' ' || *seq == '\t' || *seq == '\n' || *seq == '\r');
}

static bool is_tag_start(const char * const seq) {
	/**
	 * Returns 0 if none, 1 if normal open, 2 if close tag
	 */
	if ((seq[0] == '<') && (seq[1] == '/')) return 2;
	return (*seq == '<');
}

static int is_tag_end(const char * const seq) {
	/**
	 * Returns 0 if not any, 1 if normal close, 2 for self-closing
	 */
	if ((seq[0] == '/') && (seq[1] == '>')) return 2;
	return (*seq == '>');
}

static bool is_assoc(const char * const seq) {
	return (*seq == '=');
}

static bool is_string_term(const char * const seq) {
	return (*seq == '"');
}

static bool is_comment_start(const char * const seq) {
	return (seq[0] == '<' && seq[1] == '-' && seq[2] == '-');
}

static bool is_comment_end(const char * const seq) {
	return (seq[0] == '-' && seq[1] == '-' && seq[2] == '>');
}

/**
 * Main Parsing and Loading Functions
 * ==================================
 * 
 * These are some parser functions for simpily loading the document into memory.
 */

uint32_t DgXML2Parse(DgXML2Node * const doc, const uint32_t content_size, const char * const content) {
	/**
	 * <summary>Parse an XML document (version 2)</summary>
	 * <input name="doc">The document node that the document will be loaded into.</input>
	 * <input name="content_size">The size of the buffer.</input>
	 * <input name="content">The fully loaded buffer of the document.</input>
	 */
	
	// Lexer Nonlocals
	Dg_XMLParseToken *tokens = NULL;
	size_t token_count = 0;
	
	for (size_t i = 0; i < content_size; i++) {
		printf("\n============ Char into iter: '%c' ============\n\n", content[i]);
		
		if (is_tag_start(&content[i])) {
			tokens = token_list_expand(tokens, &token_count);
			
			// Handle </closing> tags
			if (is_tag_start(&content[i]) == 2) {
				printf("Found ending tag!\n");
				i++;
				size_t start = i;
				
				while (is_tag_end(&content[i]) != 1) {
					i++;
				}
				
				tokens[token_count - 1].type = DG_XML_TAG_END;
				tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
				
				continue;
			}
			
			printf("Found starting tag!\n");
			
			// Now we can handle normal tags
			// Tag name start location
			i++;
			size_t start = i;
			
			while (!is_whitespace(&content[i]) && !is_tag_end(&content[i])) {
				i++;
			}
			
			char *name = DgStrdupl(&content[start], i - start);
			
			ALLOC_ERROR_CHECK(name);
			
			tokens[token_count - 1].type = DG_XML_TAG_START;
			tokens[token_count - 1].text = name;
			
			printf("Found node with name: %s\n", tokens[token_count - 1].text);
			
			// Next chceck for attributes then ending of tag
			while (true) {
				printf("Key/Value Iteration!\n");
				while (is_whitespace(&content[i])) {
					i++;
				};
				
				int endtype = is_tag_end(&content[i]);
				
				if (endtype == 2) {
					tokens = token_list_expand(tokens, &token_count);
					
					tokens[token_count - 1].type = DG_XML_TAG_END;
					tokens[token_count - 1].text = DgStrdup(name);
				}
				else if (endtype == 1) {
					break;
				}
				
				do {
					i++;
				} while (is_whitespace(&content[i]));
				
				// Otherwise we can add attribute
				start = i;
				
				while (!is_assoc(&content[i])) {
					i++;
					
					if (is_whitespace(&content[i])) {
						// handle whitespace between assocaitor
						printf("\033[1;33mWarning:\033[0m XML Parser Error: Found whitespace in assocaitor.\nRest of document:\n%s\n", &content[i]);
						return 1;
					}
				}
				
				char * key = DgStrdupl(&content[start], i - start);
				
				ALLOC_ERROR_CHECK(key);
				
				// Add token for key
				tokens = token_list_expand(tokens, &token_count);
				tokens[token_count - 1].type = DG_XML_KEY;
				tokens[token_count - 1].text = key;
				
				i++;
				if (!is_string_term(&content[i])) {
					// handle this case (ie. abc=def")
					printf("\033[1;33mWarning:\033[0m XML Parser Error: Found key with malformed value (no quote after assocaitor), got %c instead.\n", content[i]);
					return 1;
				}
				i++;
				
				// Store value
				start = i;
				
				do {
					i++;
				} while (!is_string_term(&content[i]));
				
				char * value = DgStrdupl(&content[start], i - start);
				
				ALLOC_ERROR_CHECK(value);
				
				printf(" Got %s = %s.\n", key, value);
				
				// Add token for value
				tokens = token_list_expand(tokens, &token_count);
				tokens[token_count - 1].type = DG_XML_VALUE;
				tokens[token_count - 1].text = value;
			}
		}
		
		if (is_comment_start(&content[i])) {
			while (!is_comment_end(&content[i])) {
				i++;
			}
			i += 3;
		}
	}
	
	printf("Have %d tokens.\n", token_count);
	
	for (size_t i = 0; i < token_count; i++) {
		printf("\ttype=%.4d: text=%s\n", tokens[i].type, tokens[i].text);
	}
}

uint32_t DgXML2Load(DgXML2Node *doc, const char * const path) {
	char *tmp_content;
	
	// Load the file
	
	char * real_path = (char *) DgEvalPath((char *) path);
	
	if (!real_path) {
		printf("Warning: XML Parser Error: Could not load file at %s.\n", real_path);
		return 1;
	}
	
	DgFileStream *stream = DgFileStreamOpen(real_path, "rb");
	
	if (!stream) {
		printf("Warning: XML Parser Error: Could not load file at %s.\n", real_path);
		return 1;
	}
	
	DgFree(real_path);
	
	size_t doc_size = DgFileStreamLength(stream);
	tmp_content = DgAlloc(doc_size);
	
	if (!tmp_content) {
		printf("Warning: XML Parser Error: Could not allocate memory to load file.\n");
		return 1;
	}
	
	DgFileStreamRead(stream, doc_size, tmp_content);
	DgFileStreamClose(stream);
	
	const char * const content = tmp_content;
	
	return DgXML2Parse(doc, doc_size, content);
}

#undef ALLOC_ERROR_CHECK
