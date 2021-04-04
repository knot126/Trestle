/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser
 */

#include <inttypes.h>

#include "../util/alloc.h"
#include "../io/fs.h"

#include "xml.h"

#define DG_XML_BUFFER_SIZE 256

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
		DgXMLPairFree(node->attrib[i]);
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

uint32_t DgXMLDocumentLoad(DgXMLNode *doc, const char *path) {
	/**
	 * <summary>Load an XML document into memory.</summary>
	 */
	char *real_path = DgEvalPath((char *) path);
	DgFileStream *stream = DgFileStreamOpen(path, "rb");
	DgFree(real_path);
	
	if (!stream) {
		doc = NULL;
		return;
	}
	
	// Lexer nonlocals
	char buffer[DG_XML_BUFFER_SIZE];
	
	
	
	DgFileStreamClose(stream);
}

