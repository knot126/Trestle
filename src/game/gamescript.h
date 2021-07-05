/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game Script
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "util/script.h"

typedef struct GameScript {
	DgScript script;
	char *load_next;
	bool active;
} GameScript;

GameScript *game_script_active(GameScript *gs);

void game_script_new(GameScript *gs);
void game_script_free(GameScript *gs);
void game_script_yeild(GameScript * restrict gs, const char * restrict name);
void game_script_update(GameScript *gs);

void game_script_script_functions(DgScript *script);
