/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file should expose ONLY game_main.
 */

#pragma once

#include "graphics/graphics.h"
#include "util/bag.h"

typedef struct {
	GraphicsInitInfo graphics;
} SystemStates;

int game_main(int argc, char* argv[]);
