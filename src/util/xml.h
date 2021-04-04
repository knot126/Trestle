/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser
 */

#pragma once

typedef struct DgXMLPair {
	char *key, *value;
} DgXMLPair;

typedef struct DgXMLNode {
	char *name;
	struct DgXMLNode **sub;
	size_t sub_count;
	DgXMLPair *attrib;
	size_t attrib_count;
	char *text;
} DgXMLNode;

uint32_t DgXMLLoad(DgXMLNode *doc, const char *path);
