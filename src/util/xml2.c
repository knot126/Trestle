/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A2
 */

#include <string.h>
#include <stdio.h>

#include "xml2.h"

typedef struct {
	enum {
		DG_XML_UNKNOWN,     // unknwon
		DG_XML_WHITESPACE,  // whitespace
		DG_XML_TAG_START,   // <tag>
		DG_XML_TAG_END,     // the />
		DG_XML_TAG_DOWN,    // </tag>
		DG_XML_NAME,        // any string
		DG_XML_VALUE,       // values
		DG_XML_ASSOC,       // the '='
		DG_XML_PROC_START,  // <?
		DG_XML_PROC_END,    // ?>
	} type;
	char *text;
} DgXMLToken;

uint32_t DgXMLLoad(DgXMLNode *doc, const char * const path) {
	
}
