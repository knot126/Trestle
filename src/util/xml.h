/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A2
 */

#pragma once

#include <inttypes.h>

typedef enum {
	DG_XML_NODE_ELEMENT = 1,
	DG_XML_NODE_PROCESS = 2,
	DG_XML_NODE_COMMENT = 3,
	DG_XML_NODE_TEXT = 4,
} DgXMLNodeType;

typedef struct {
	char *key;
	char *value;
} DgXMLPair;

typedef struct DgXMLNode {
	// Node name
	char *name;
	
	// Node attributes
	DgXMLPair *attrib;
	size_t attrib_count;
	
	// Subnodes
	struct DgXMLNode *sub;
	size_t sub_count;
	
	// Node text
	char *text;
} DgXMLNode;

uint32_t DgXMLParse(DgXMLNode * const doc, const uint32_t content_size, const char * const content);
uint32_t DgXMLLoadString(DgXMLNode *doc, const char * const str);
uint32_t DgXMLLoad(DgXMLNode *doc, const char * const path);
void DgXMLNodeFree(DgXMLNode *node);
void DgXMLPrintNode(const DgXMLNode * const doc, uint32_t depth);
char *DgXMLGetAttrib(DgXMLNode *node, const char * const restrict key, char *fallback);
