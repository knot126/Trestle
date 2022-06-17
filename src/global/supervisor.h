/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the systems
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "game/scriptman.h"
#include "global/reg.h"
#include "graphics/graphics.h"
#include "graph/graph.h"
#include "input/input.h"

enum {
	ENT_TRANSFORM = (1 << 0),
	
	ENT_GRAPHICS_MESH = (1 << 1),	
	ENT_GRAPHICS_MESH2D = (1 << 7),
	ENT_GRAPHICS_SURFACE = (1 << 8),
};

typedef struct Supervisor {
	/**
	 * Structure containing all of the system states.
	 */
	
	// G
	GraphicsSystem graphics;
	// I
	InputSystem input;
	// R
	Registry reg;
	// S
	SceneGraph graph;
	ScriptManager scriptman;
	
	// very basic gloal state
	Name next;
	bool running;
} Supervisor;

Supervisor *supervisor(Supervisor *S);

void sup_init(Supervisor * restrict sup);
void sup_destroy(Supervisor * restrict sup);

void sup_close(Supervisor *sup);

Name sup_next_name(Supervisor *sup);
Name sup_entity(Supervisor *sup, const uint64_t systems);
