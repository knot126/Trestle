/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser
 */

#include <stdio.h>
#include <inttypes.h>

#include "../util/alloc.h"
#include "../util/fs.h"

#include "xml.h"

/**
 * Parser Helper Functions
 * =======================
 * 
 * Functions that helps with creating and freeing parser structures.
 */

static void DgXMLPairFree(DgXMLPair *pair) {
	/**
	 * <summary>Frees a DgXMLPair.</summary>
	 */
	
	DgFree(pair->key);
	DgFree(pair->value);
}

static void DgXMLNodeFree(DgXMLNode *node) {
	/**
	 * <summary>Frees a DgXMLNode.</summary>
	 */
	
	DgFree(node->name);
	
	for (size_t i = 0; i < node->sub_count; i++) {
		DgXMLNodeFree(node->sub[i]);
	}
	
	DgFree(node->sub);
	
	for (size_t i = 0; i < node->attrib_count; i++) {
		DgXMLPairFree((node->attrib + i));
	}
	
	DgFree(node->attrib);
	DgFree(node->text);
}

/**
 * Main Parser Functions: Load Full Document
 * =========================================
 * 
 * These are some parser functions for simpily loading the document into memory.
 * There are/will be some other APIs that can take care of traversing the
 * document in a more iterator-friendly enviornment. See the wiki page about XML
 * parser APIs.
 */

uint32_t DgXMLLoad(DgXMLNode *doc, const char *path) {
	/**
	 * <summary>Load an XML document into memory; specifically, into the <type>DgXMLNode.</summary>
	 */
	
	char *real_path = DgEvalPath((char *) path);
	DgFileStream *stream = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!stream) {
		return 1;
	}
	
	// Load document into memory and close the stream
	size_t doc_size = DgFileStreamLength(stream);
	char * const content = DgAlloc(doc_size + 1);
	
	if (!content) {
		return 2;
	}
	
	DgFileStreamRead(stream, doc_size, content);
	content[doc_size] = '\0';
	DgFileStreamClose(stream);
	
	printf("%s\n", content);
	
	// Lexer nonlocals
	char *current = (char *) content;
	
	// Lexer to parse the document
	for (size_t i = 0; i < doc_size; i++) {
		
	}
	
	return 0;
}

