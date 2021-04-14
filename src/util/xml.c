/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A1
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../util/alloc.h"
#include "../util/fs.h"

#include "xml.h"

const bool _DG_XML_DEBUG = false;

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

static void DgXMLNodeFree(DgXMLNode node) {
	/**
	 * <summary>Frees a DgXMLNode.</summary>
	 */
	
	DgFree(node.name);
	
	for (size_t i = 0; i < node.sub_count; i++) {
		DgXMLNodeFree(node.sub[i]);
	}
	
	DgFree(node.sub);
	
	for (size_t i = 0; i < node.attrib_count; i++) {
		DgXMLPairFree((node.attrib + i));
	}
	
	DgFree(node.attrib);
	/*
	DgFree(node.text);
	*/
}

/**
 * Test Functions
 * ==============
 * 
 * Functions for tests like isWhitespace()
 */

static bool isWhitespace(char c) {
	// Is XML whitespace?
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static bool isBom(const char * const c) {
	// Is UTF-8 BOM?
	return (c[0] == '\xEF' && c[1] == '\xBB' && c[2] == '\xBF');
}

static bool isStart(char c) {
	return (c == '<');
}

static bool isEnd(char c) {
	return (c == '>' || c == '/');
}

static bool isNodeEnd(char c) {
	return (c == '/');
}

static bool isSeperator(char c) {
	return (c == '=');
}

static bool isString(char c) {
	return (c == '"');
}

static bool isCommentStart(const char * const c) {
	return (c[0] == '<' && c[1] == '!' && c[2] == '-' && c[3] == '-');
}

static bool isCommentEnd(const char * const c) {
	return (c[0] == '-' && c[1] == '-' && c[2] == '>');
}

static bool isProcInst(const char * const c) {
	return (c[0] == '<' && c[1] == '?');
}

static bool isProcInstEnd(const char * const c) {
	return (c[0] == '?' && c[1] == '>');
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

uint32_t DgXMLLoad(DgXMLNode * const doc, const char * const path) {
	/**
	 * <summary>Load an XML document into memory; specifically, into the <type>DgXMLNode</type> provided.</summary>
	 * <input name="doc">The root node that the document will be loaded into.</input>
	 * <input name="path">The path that will be passed to <func file="fs.c">DgEvalPath</func>.</input>
	 */
	
	char *real_path = DgEvalPath((char *) path);
	
	if (!real_path) {
		return 1;
	}
	
	DgFileStream *stream = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!stream) {
		return 1;
	}
	
	// Load document into memory and close the stream
	size_t doc_size = DgFileStreamLength(stream);
	char * const content = (char * const) DgAlloc(doc_size + 1);
	
	if (!content) {
		return 2;
	}
	
	DgFileStreamRead(stream, doc_size, content);
	content[doc_size] = '\0';
	DgFileStreamClose(stream);
	
	if(_DG_XML_DEBUG) printf("== %s ==\n%s\n", path, content);
	
	// Clear the XML node
	memset(doc, 0, sizeof(DgXMLNode));
	
	// Parser nonlocals
	uint32_t depth = 0;
	DgXMLNode *current = doc;
	
	// Parser, to parse the document (this used to say lexer and not parser :P)
	for (size_t i = 0; i < doc_size; i++) {
		if(_DG_XML_DEBUG) printf("## START OF LOOP ##\nChar into iter: %c\n", content[i]);
		
		// Update the current node to be sure it's correct
		current = doc;
		for (uint32_t i = 1; i < depth; i++) {
			current = &current->sub[current->sub_count - 1];
		}
		
		// Check for comments
		if (isCommentStart(&content[i])) {
			if(_DG_XML_DEBUG) printf("Comment start at %d.\n", i);
			do {
				i++;
			} while (!isCommentEnd(&content[i]) && content[i] != '\0');
			if(_DG_XML_DEBUG) printf("Comment end at %d.\n", i);
		}
		
		// Start of a node
		if (isStart(content[i])) {
			if(_DG_XML_DEBUG) printf("Entering start of node. Current char = '%c'.\n", content[i]);
			
			// Go to tag name
			do {
				i++;
			} while (isWhitespace(content[i]));
			
			// Independent closing tags
			if (isNodeEnd(content[i])) {
				depth--;
				if (depth == 0) {
					break;
				}
				do {
					i++;
				} while (!isEnd(content[i]));
				continue;
			}
			
			depth++;
			
			// Allocate subnodes
			if (depth > 1) {
				current->sub_count++;
				current->sub = (DgXMLNode *) DgRealloc(current->sub, sizeof(DgXMLNode) * current->sub_count);
				current = &current->sub[current->sub_count - 1];
				memset(current, 0, sizeof(DgXMLNode));
			}
			
			// Get tag name
			size_t start = i;
			while (!isWhitespace(content[i]) && !isEnd(content[i])) {
				i++;
			}
			bool end = isEnd(content[i]);
			content[i] = '\0';
			current->name = DgStrdup(&content[start]);
			if (_DG_XML_DEBUG) {
				printf("(%d) %s\n", depth, current->name);
			}
			
			if (end) {
				continue;
			}
			
			while (true/*!isEnd(content[i])*/) {
				do {
					i++;
				} while (isWhitespace(content[i]));
				
				if (isEnd(content[i])) {
					if (isNodeEnd(content[i])) {
						depth--;
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
					printf("Warning: XML Parser Error: Expected start of string, got '%c'.\n", content[i]);
					return 5;
				}
				i++;
				
				// Get value
				start = i;
				do {
					i++;
				} while (!isString(content[i]));
				content[i] = '\0';
				char *value = DgStrdup(&content[start]);
				
				if (_DG_XML_DEBUG) {
					printf("\t%s : %s\n", key, value);
				}
				
				// Add pair to current node
				current->attrib_count++;
				current->attrib = DgRealloc(current->attrib, sizeof(DgXMLPair) * current->attrib_count);
				current->attrib[current->attrib_count - 1].key = key;
				current->attrib[current->attrib_count - 1].value = value;
			}
		}
		
		if (_DG_XML_DEBUG) {
			printf("Next depth: %d\n", depth);
			printf("Rest of Document: %s\n", &content[i]);
		}
		
		// NOTERA: Does not check that enough of the document is left to check for BOM.
		if (depth == 0 && !isWhitespace(content[i]) && !isBom(&content[i])) {
			printf("Warning: XML Parser Error: Junk comes before document.\n");
			return 3;
		}
		
		if (isWhitespace(content[i])) {
			// Do nothing
			if(_DG_XML_DEBUG) printf("Skipping characther.\n");
		}
	}
	
	if (depth != 0) {
		printf("Warning: XML Parser Error: Soft assertion (depth != 0) failed!\n");
		printf("Warning: Hint: Make sure the document hiearchy is okay (tags end correctly).\n");
		return 6;
	}
	
	DgFree(content);
	
	return 0;
}

void DgXMLFree(DgXMLNode *doc) {
	DgXMLNodeFree(*doc);
}

void DgXMLPrintNode(uint32_t indent, DgXMLNode* node) {
	if (node->name) {
		for (uint32_t i = 0; i < indent; i++) {
			printf("\t");
		}
		
		printf("%s:\n", node->name);
	}
	
	indent++;
	
	if (node->attrib) {
		for (uint32_t i = 0; i < node->attrib_count; i++) {
			for (uint32_t j = 0; j < indent; j++) {
				printf("\t");
			}
			printf("%s = \"%s\"\n", node->attrib[i].key, node->attrib[i].value);
		}
	}
	
	if (node->sub) {
		for (uint32_t i = 0; i < node->sub_count; i++) {
			DgXMLPrintNode(indent, &node->sub[i]);
		}
	}
}

