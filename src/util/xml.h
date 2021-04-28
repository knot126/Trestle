/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A1
 */

#pragma once

typedef struct DgSimpleXMLPair {
	char *key, *value;
} DgSimpleXMLPair;

typedef struct DgSimpleXMLNode {
	char *name;
	struct DgSimpleXMLNode *sub;
	size_t sub_count;
	DgSimpleXMLPair *attrib;
	size_t attrib_count;
} DgSimpleXMLNode;

uint32_t DgSimpleXMLParse(DgSimpleXMLNode * const doc, uint32_t t_doc_size, char * const content);
uint32_t DgSimpleXMLLoad(DgSimpleXMLNode * const doc, const char * const path);
void DgSimpleXMLFree(DgSimpleXMLNode *doc);
void DgSimpleXMLPrintNode(uint32_t indent, DgSimpleXMLNode* node);
