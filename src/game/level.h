/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Level System
 */

#pragma once

#include <inttypes.h>

#include "util/script.h"

typedef struct Room {
	DgScript script;
} Room;

typedef struct LevelSystem {
	Room room_script[2];
	char **level;
	char **room;
	
	uint32_t level_count;
	uint32_t room_count;
	uint32_t clear_after;
} LevelSystem;

uint32_t level_init(LevelSystem * restrict ls, const char * const restrict game);
void level_free(LevelSystem * restrict ls);
void level_update(World * const restrict world, LevelSystem * const restrict ls);
