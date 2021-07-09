/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the systems
 */

#pragma once

#include "game/level.h"
#include "game/gamescript.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "world/world.h"

typedef struct Supervisor {
	/**
	 * Structure containing all of the system states.
	 */
	
	// G
	GameScript game_script;
	GraphicsSystem graphics;
	// I
	InputSystem input;
	// L
	LevelSystem level_info;
} Supervisor;

Supervisor *supervisor(Supervisor *S);

void sup_init(Supervisor * restrict sup, const World * restrict world);
void sup_destroy(Supervisor * restrict sup);
