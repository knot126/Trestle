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
#include "util/rand.h"
#include "world/scripting.h"
#include "world/world.h"

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
			ls->level = (char **) DgRealloc(ls->level, sizeof *ls->level * ls->level_count);
			
			if (!ls->level) {
				DgLog(DG_LOG_ERROR, "Failed to allocate memory.");
				return 1;
			}
			
			ls->level[ls->level_count - 1] = DgStrdup(DgXMLGetAttrib(&root.sub[i], "name", ""));
		}
	}
	
	// Print levels names
	for (size_t i = 0; i < ls->level_count; i++) {
		DgLog(DG_LOG_VERBOSE, "Have level: %s", ls->level[i]);
	}
	
	DgXMLNodeFree(&root);
	
	return 0;
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

void level_update(World * const restrict world, LevelSystem * const restrict ls) {
	/**
	 * Update the level system and any levels in it.
	 */
	
	DgVec3 playerpos = world_get_player_position(world);
	
	// If there are no more rooms left, then load the next level file
	if (!ls->room || !ls->room_count) {
		DgXMLNode root;
		
		// Get the path name
		char *a = DgStrcadf(DgStrcad("assets://levels/", ls->level[0]), ".xml");
		DgXMLLoad(&root, a);
		DgFree(a);
		
		// Find all the rooms
		for (size_t i = 0; i < root.sub_count; i++) {
			if (!strcmp(root.sub[i].name, "room")) {
				ls->room_count++;
				ls->room = (char **) DgRealloc(ls->room, sizeof *ls->room * ls->room_count);
				
				if (!ls->room) {
					DgLog(DG_LOG_ERROR, "Failed to allocate memory (in loading level).");
					return;
				}
				
				ls->room[ls->room_count - 1] = DgStrdup(DgXMLGetAttrib(&root.sub[i], "name", ""));
			}
		}
		
		DgXMLNodeFree(&root);
		
		// Pop the level off of the stack, unless it is the last one remaining
		if (ls->room_count > 1) {
			
		}
	}
	
	// Start to load the next room when approaching it
	if (-playerpos.z >= (world->game.load_next - 25.0f)) {
		world->game.load_next += world->game.new_length;
		
		// Init script, register functions
		DgScriptInit(&ls->room_script[1].script);
		DgRegisterRandFuncs(&ls->room_script[1].script);
		registerWorldScriptFunctions(&ls->room_script[1].script);
		
		// Load room at next then free
		DgScriptLoad(&ls->room_script[1].script, "assets://rooms/include.lua");
		char *a = DgStrcadf(DgStrcad("assets://rooms/", ls->room[0]), ".lua");
		
		if (!a) {
			DgLog(DG_LOG_ERROR, "Failed to allocate memory (while loding room).");
			return;
		}
		
		DgLog(DG_LOG_INFO, "Loading room at '%s'.", a);
		
		DgScriptLoad(&ls->room_script[1].script, a);
		DgFree(a);
		
		// Call init
		DgScriptCall(&ls->room_script[1].script, "init");
		
		// Pop the room off of the stack
		ls->room_count--;
		
		DgFree(ls->room[0]); // Free memory
		
		if (ls->room_count > 0) {
			memmove(ls->room, (ls->room + 1), sizeof *ls->room * ls->room_count);
		}
		
		ls->room = (char **) DgRealloc(ls->room, sizeof *ls->room * ls->room_count);
		
		if (!ls->room) {
			DgLog(DG_LOG_WARNING, "Failed to allocate memory (list of rooms), or out of rooms for this level.");
		}
	}
	
	// Set that room as the primary room and delete the old one
	if (-playerpos.z >= (world->game.load_next - 0.01f)) {
		if (ls->room_script[0].script.state) {
			DgScriptCall(&ls->room_script[0].script, "free");
			DgScriptFree(&ls->room_script[0].script);
		}
		ls->room_script[0] = ls->room_script[1];
	}
	
	// Call the room's tick method
	DgScriptCall(&ls->room_script[0].script, "tick");
}
