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

typedef struct {
	GraphicsInitInfo graphics;
	DgScript game_script;
} SystemStates;

int game_main(int argc, char* argv[]);
