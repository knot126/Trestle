/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the systems
 */

#pragma once

#include "game/gamescript.h"
#include "graphics/graphics.h"
#include "graph/graph.h"
#include "input/input.h"

typedef struct Supervisor {
	/**
	 * Structure containing all of the system states.
	 */
	
	// G
	GameScript game_script;
	GraphicsSystem graphics;
	// I
	InputSystem input;
	// S
	SceneGraph graph;
} Supervisor;

Supervisor *supervisor(Supervisor *S);

void sup_init(Supervisor * restrict sup);
void sup_destroy(Supervisor * restrict sup);
