/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games INI Parser v1
 */

#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "alloc.h"
#include "fs.h"

#include "ini.h"

/**
 * Private Data Types
 * ==================
 */

typedef struct Dg_INIToken {
	enum {
		DG_INI_NULL,
		DG_INI_SECTION,
		DG_INI_KEY,
		DG_INI_ASSOC,
		DG_INI_VALUE,
	} type;
	char * text;
} Dg_INIToken;

/**
 * Helper functions
 * ================
 */

static void token_expand(Dg_INIToken **tokens, size_t *count) {
	*count = *count + 1;
	*tokens = (Dg_INIToken *) DgRealloc(*tokens, *count * sizeof(Dg_INIToken));
}

static bool is_whitespace(const char * const c) {
	return (*c == ' ' || *c == '\n' || *c == '\t' || *c == '\r');
}

static bool is_section(const char * const c) {
	return (*c == '[' || *c == ']');
}

static bool is_assoc(const char * const c) {
	return (*c == '=');
}

static bool is_comment(const char * const c) {
	return (*c == '#' || *c == ';');
}

static bool is_escape(const char * const c) {
	return (*c == '\\');
}

static bool is_newline(const char * const c) {
	return (*c == '\n' || *c == '\r');
}

static char get_real_char(const char * const c) {
	switch (*c) {
		case 'n':
			return '\n';
		case 'r':
			return '\r';
		case 't':
			return '\t';
		case '#':
			return '#';
		case ';':
			return ';';
		case '\\':
			return '\\';
	}
	
	return *c;
}

/**
 * Actual Parsing and Loading Functions
 */

uint32_t DgINIParse(DgINIDocument *doc, const uint32_t length, const char * const content) {
	/**
	 * Parse an INI file from a buffer of an already known size.
	 */
	
	Dg_INIToken *tokens = NULL;
	size_t token_count = 0;
	
	for (size_t i = 0; i < length; i++) {
		if (is_comment(&content[i])) {
			while (i < length && !is_newline(&content[i])) {
				i++;
			}
		}
		
		else if (is_whitespace(&content[i])) {
			continue;
		}
		
		else if (is_assoc(&content[i])) {
			token_expand(&tokens, &token_count);
			
			if (!tokens) {
				return 1;
			}
			
			tokens[token_count - 1].type = DG_INI_ASSOC;
			tokens[token_count - 1].text = NULL;
		}
		
		else if (is_section(&content[i])) {
			token_expand(&tokens, &token_count);
			
			if (!tokens) {
				return 1;
			}
			
			i++;
			size_t start = i;
			
			do {
				i++;
			} while (i < length && !is_section(&content[i]));
			
			tokens[token_count - 1].type = DG_INI_SECTION;
			tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
		}
		
		else {
			// skip whitespace before
			while (i < length && is_whitespace(&content[i])) {
				i++;
			}
			
			size_t start = i;
			
			while (i < length && !is_assoc(&content[i]) && !is_newline(&content[i])) 
			{
				i++;
			}
			
			// remove extra whitespace after assoc
			if (is_assoc(&content[i])) {
				while (i > 0 && is_whitespace(&content[i - 1])) {
					i--;
				};
			}
			
			token_expand(&tokens, &token_count);
			
			if (!tokens) {
				return 1;
			}
			
			tokens[token_count - 1].type = DG_INI_VALUE;
			tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
		}
	}
	
	// Clear the document node
	memset(doc, 0, sizeof(DgINIDocument));
	
	for (size_t i = 0; i < token_count; i++) {
// 		printf("[ %.3d ] %d : \"%s\"\n", i, tokens[i].type, tokens[i].text);
		
		if (tokens[i].type == DG_INI_SECTION) {
			doc->section_count = doc->section_count + 1;
			doc->sections = DgRealloc(doc->sections, doc->section_count * sizeof(DgINISection));
			
			if (!doc->sections) {
				return 2;
			}
			
			DgINISection *sect = &doc->sections[doc->section_count - 1];
			memset(sect, 0, sizeof(DgINISection));
			
			sect->title = tokens[i].text;
			
			// Values
			while ((i + 3) < token_count && tokens[i + 1].type != DG_INI_SECTION) {
				i++;
				
				if (tokens[i].type != DG_INI_VALUE) {
					printf("\033[1;33mWarning:\033[0m INI Parser Error: Expected VALUE for key.\n");
					return 2;
				}
				
				char *key = tokens[i].text;
				
				i++;
				
				if (tokens[i].type != DG_INI_ASSOC) {
					printf("\033[1;33mWarning:\033[0m INI Parser Error: Expected assocaitor for values.\n");
					return 2;
				}
				
				i++;
				
				if (tokens[i].type != DG_INI_VALUE) {
					printf("\033[1;33mWarning:\033[0m INI Parser Error: Expected value.\n");
					return 2;
				}
				
				char *value = tokens[i].text;
				
				sect->pair_count = sect->pair_count + 1;
				sect->pairs = DgRealloc(sect->pairs, sect->pair_count * sizeof(DgINIPair));
				
				sect->pairs[sect->pair_count - 1].key = key;
				sect->pairs[sect->pair_count - 1].value = value;
			}
		}
	}
	
	DgFree(tokens);
}

uint32_t DgINILoad(DgINIDocument *doc, char * const path) {
	/**
	 * Load and parse an INI file. Return non-zero on an error, like all other
	 * file parsers in Melon.
	 */
	
	// Load the file
	char *real_path = DgEvalPath(path);
	DgFileStream *f = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!f) {
		return 1;
	}
	
	size_t length = DgFileStreamLength(f);
	
	// Allocate for loading document
	char *content = (char *) DgAlloc(length * sizeof(char));
	
	if (!content) {
		return 1;
	}
	
	DgFileStreamRead(f, length * sizeof(char), content);
	
	DgFileStreamClose(f);
	
	// Parse the doucment
	uint32_t status = DgINIParse(doc, length, content);
	
	DgFree(content);
	
	return status;
}

void DgINIFree(DgINIDocument *doc) {
	/**
	 * Free and INI document
	 */
	for (size_t i = 0; i < doc->section_count; i++) {
		DgFree(doc->sections[i].title);
		
		for (size_t j = 0; j < doc->sections[i].pair_count; j++) {
			DgFree(doc->sections[i].pairs[j].key);
			DgFree(doc->sections[i].pairs[j].value);
		}
		
		DgFree(doc->sections[i].pairs);
	}
	
	DgFree(doc->sections);
}

/**
 * Functions for Getting/Setting data in INI files
 */

char *DgINIGet(DgINIDocument *doc, const char * const sect, const char * const key) {
	/**
	 * Get a value from and INI document, returns empty string if none.
	 */
	
	char *result = "";
	
	for (size_t i = 0; i < doc->section_count; i++) {
		if (!strcmp(doc->sections[i].title, sect)) {
			DgINISection *section = &doc->sections[i];
// 			printf("Found section with title %s.\n", section->title);
			
			for (size_t j = 0; j < section->pair_count; j++) {
				if (!strcmp(section->pairs[j].key, key)) {
// 					printf("Found key with value %s.\n", section->pairs[j].value);
					result = section->pairs[j].value;
				}
			}
		}
	}
	
	return result;
}

void DgINIPrint(DgINIDocument *doc) {
	for (size_t i = 0; i < doc->section_count; i++) {
		DgINISection *section = &doc->sections[i];
		
		printf("[%s]\n", section->title);
		
		for (size_t j = 0; j < section->pair_count; j++) {
			printf("\t%s = %s\n", section->pairs[j].key, section->pairs[j].value);
		}
	}
}
