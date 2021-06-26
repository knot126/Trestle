/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Level System
 */

#include <string.h>

#include "util/alloc.h"
#include "util/script.h"
#include "util/fs.h"
#include "util/xml.h"
#include "util/str.h"
#include "util/log.h"

#include "level.h"

uint32_t level_init(LevelSystem * restrict ls, const char * const restrict game) {
	/**
	 * Initialise a level system and load the checkpoint index at the given 
	 * path.
	 */
	
	memset(ls, 0, sizeof *ls);
	
	DgXMLNode root;
	
	uint32_t status = DgXMLLoad(&root, game);
	
	if (status) {
		DgXMLNodeFree(&root);
		return status;
	}
	
	for (size_t i = 0; i < root.sub_count; i++) {
		if (!strcmp(root.sub[i].name, "level")) {
			// Add the level to the list
			ls->level_count++;
			ls->level = DgRealloc(ls->level, sizeof *ls->level * ls->level_count);
			ls->level[ls->level_count - 1] = DgStrdup(DgXMLGetAttrib(&root.sub[i], "name", ""));
		}
	}
	
	// Print levels names
	for (size_t i = 0; i < ls->level_count; i++) {
		DgLog(DG_LOG_VERBOSE, "Have level: %s", ls->level[i]);
	}
	
	DgXMLNodeFree(&root);
}

void level_free(LevelSystem * restrict ls) {
	/**
	 * Release any memory used by a level system.
	 */
	
	if (ls->level) {
		for (size_t i = 0; i < ls->level_count; i++) {
			DgFree(ls->level[i]);
		}
	}
	
	if (ls->room) {
		for (size_t i = 0; i < ls->room_count; i++) {
			DgFree(ls->room[i]);
		}
	}
}
