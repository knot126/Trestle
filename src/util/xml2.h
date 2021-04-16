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
} DgXML2Pair;

typedef struct DgXMLNode {
	char *name;
	DgXML2Pair *attrib;
	struct DgXMLEntity *sub;
	char *text;
} DgXML2Node;
