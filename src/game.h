/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file should expose ONLY game_main.
 */

#pragma once

#include "graphics/graphics.h"
#include "util/script.h"
#include "util/bag.h"
#include "game/level.h"
#include "game/gamescript.h"

typedef struct {
	GraphicsInitInfo graphics;
	GameScript game_script;
	LevelSystem level_info;
} SystemStates;

int game_main(int argc, char* argv[]);
