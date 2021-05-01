/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games INI Parser
 */

#include <string.h>
#include <stdbool.h>

#include "alloc.h"
#include "fs.h"

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

uint32_t DgINIParse(DgINIDocument *doc, uint32_t length, const char * const content) {
	
}

uint32_t DgINILoad(DgINIDocument *doc, char * const path) {
	char *real_path = DgEvalPath(path);
	
	DgFileStream *f = DgFileStreamOpen(real_path, "rb");
	
	DgFree(real_path);
}
