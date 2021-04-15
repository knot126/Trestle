/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A2
 */

#pragma once

#include <inttypes.h>

typedef struct {
	char *key;
	char *value;
} DgXMLPair;

typedef struct DgXMLNode {
	char *name;
	DgXMLPair *attrib;
	struct DgXMLEntity *sub;
	char *text;
} DgXMLNode;
