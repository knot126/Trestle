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
	struct DgXMLNode *sub;
	DgXMLPair *attributes;
	char *text;
} DgXMLNode;

typedef struct DgXMLDocument {
	char *name;
	DgXMLNode *sub;
	DgXMLPair *attributes;
	char *text;
} DgXMLDocument;
