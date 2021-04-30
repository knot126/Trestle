/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Segment Mangement
 */

#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include "world.h"
#include "../util/alloc.h"
#include "../util/xml.h"
#include "../util/fs.h"
#include "../util/maths.h"

float next;

float segment_load(const char * const path) {
	/**
	 * Load a segment from a file into the current world.
	 * 
	 * Returns its length.
	 */
	DgXMLNode segment;
	
	uint32_t status = DgXMLLoad(&segment, path);
	
	if (status) {
		return 0.0f;
	}
	
	// Make sure this is a segment file
	if (strcmp(segment.name, "segment")) {
		printf("\033[1;33mWarning:\033[0m Failed to load segment at %s: not a segment file.\n", path);
		return 0.0f;
	}
	
	DgVec3 len = DgVec3New(0.0f, 0.0f, 0.0f);
	
	for (uint32_t i = 0; i < segment.attrib_count; i++) {
		if (!strcmp(segment.attrib[i].key, "size")) {
			len = DgVec3FromString(segment.attrib[i].value);
			break;
		}
	}
	
	next -= len.z;
	
	for (uint32_t i = 0; i < segment.sub_count; i++) {
		char * name = segment.sub[i].name;
		
		if (!strcmp(name, "box")) {
			DgVec3 pos, size;
			
			// Find all of the data needed
			
		}
	}
	
	DgXMLNodeFree(&segment);
	
	return len.z;
}
