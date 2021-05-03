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

uint32_t DgINIParse(DgINIDocument *doc, uint32_t length, const char * const content) {
	/**
	 * Parse an INI file from a buffer of an already known size.
	 */
	
	Dg_INIToken *tokens = NULL;
	size_t token_count = 0;
	
	for (size_t i = 0; i < length; i++) {
		
	}
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
