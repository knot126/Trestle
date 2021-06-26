/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Level System
 */

#pragma once

#include <inttypes.h>

#include "util/script.h"

typedef struct LevelSystem {
	DgScript level_script[2];
	char **level;
	char **room;
	
	uint32_t level_count;
	uint32_t room_count;
} LevelSystem;

uint32_t level_init(LevelSystem * restrict ls, const char * const restrict game);
void level_free(LevelSystem * restrict ls);
