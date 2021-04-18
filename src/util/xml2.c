/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A2
 */

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
		DG_XML_UNKNOWN,     // unknwon
		DG_XML_WHITESPACE,  // whitespace
		DG_XML_TAG_START,   // <tag>
		DG_XML_TAG_END,     // the />
		DG_XML_TAG_DOWN,    // </tag>
		DG_XML_NAME,        // any string
		DG_XML_VALUE,       // values
		DG_XML_ASSOC,       // the '='
		DG_XML_PROC_START,  // <?
		DG_XML_PROC_END,    // ?>
	} type;
	char *text;
} DgXML2Token;

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
	DgXML2Token *token_list = NULL;
	size_t depth = 0;
	
	for (size_t i = 0; i < content_size; i++) {
		
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
	
	return DgXML2Parse(NULL, 0, NULL);
}
