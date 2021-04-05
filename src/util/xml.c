/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

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
 * Test Functions
 * ==============
 * 
 * Functions for tests like isWhitespace()
 */

static bool isWhitespace(char c) {
	// Is XML whitespace?
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
		return true;
	}
	else {
		return false;
	}
}

static bool isBom(const char * const c) {
	// Is UTF-8 BOM?
	if (c[0] == '\xEF' && c[1] == '\xBB' && c[2] == '\xBF') {
		return true;
	}
	else {
		return false;
	}
}

static bool isStart(char c) {
	if (c == '<') {
		return true;
	}
	else {
		return false;
	}
}

static bool isEnd(char c) {
	if (c == '>' || c == '/') {
		return true;
	}
	else {
		return false;
	}
}

static bool isNodeEnd(char c) {
	if (c == '/') {
		return true;
	}
	else {
		return false;
	}
}

static bool isSeperator(char c) {
	if (c == '=') {
		return true;
	}
	else {
		return false;
	}
}

static bool isString(char c) {
	if (c == '"') {
		return true;
	}
	else {
		return false;
	}
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
	uint32_t depth = 0;
	
//#if 0
	// Lexer to parse the document
	for (size_t i = 0; i < doc_size; i++) {
		if (isStart(content[i])) {
			// Go to tag name
			do {
				i++;
			} while (isWhitespace(content[i]));
			
			// Get tag name
			size_t start = i;
			while (!isWhitespace(content[i])) {
				i++;
			}
			content[i] = '\0';
			doc->name = DgStrdup(&content[start]);
			printf("%s\n", doc->name);
			
			while (!isEnd(content[i]) || !isNodeEnd(content[i])) {
				do {
					i++;
				} while (isWhitespace(content[i]));
				
				if (isEnd(content[i])) {
					if (!isNodeEnd(content[i])) {
						depth++;
					}
					break;
				}
				
				// get value
				start = i;
				while (!isWhitespace(content[i]) && !isSeperator(content[i])) {
					i++;
				}
				content[i] = '\0';
				char *key = DgStrdup(&content[start]);
				
				// We should expect a starting quote
				i++;
				if (!isString(content[i])) {
					printf("Warning: XML Parser Error: Expected start of string, got %c.\n", content[i]);
					return 5;
				}
				i++;
				
				// 
				start = i;
				do {
					i++;
				} while (!isString(content[i]));
				content[i] = '\0';
				char *value = DgStrdup(&content[start]);
				
				printf("\t%s : %s\n", key, value);
			}
		}
		
		// NOTERA: Does not check that enough of the document is left to check for BOM.
		if (depth == 0 && !isWhitespace(content[i]) && !isBom(&content[i])) {
			printf("Warning: XML Parser Error: Junk comes before document.\n");
			return 3;
		}
		
		if (isWhitespace(content[i])) {
			i++;
		}
	}
//#endif
	
	return 0;
}

