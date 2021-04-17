/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A1
 */

#pragma once

typedef struct DgXMLPair {
	char *key, *value;
} DgXMLPair;

typedef struct DgXMLNode {
	char *name;
	struct DgXMLNode *sub;
	size_t sub_count;
	DgXMLPair *attrib;
	size_t attrib_count;
} DgXMLNode;

uint32_t DgXMLParse(DgXMLNode * const doc, uint32_t t_doc_size, char * const content);
uint32_t DgXMLLoad(DgXMLNode * const doc, const char * const path);
void DgXMLFree(DgXMLNode *doc);
void DgXMLPrintNode(uint32_t indent, DgXMLNode* node);
