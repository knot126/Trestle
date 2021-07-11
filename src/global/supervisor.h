/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the systems
 */

#pragma once

#include <inttypes.h>

#include "game/gamescript.h"
#include "graphics/graphics.h"
#include "graph/graph.h"
#include "physics/physics.h"
#include "input/input.h"

enum {
	ENT_TRANSFORM = (1 << 0),
	ENT_GRAPHICS_MESH = (1 << 1),
	ENT_GRAPHICS_CURVE = (1 << 2),
};

typedef struct Supervisor {
	/**
	 * Structure containing all of the system states.
	 */
	
	// G
	GameScript game_script;
	GraphicsSystem graphics;
	// I
	InputSystem input;
	// P
	PhysicsSystem physics;
	// S
	SceneGraph graph;
	
	Name next;
} Supervisor;

Supervisor *supervisor(Supervisor *S);

void sup_init(Supervisor * restrict sup);
void sup_destroy(Supervisor * restrict sup);

Name sup_next_name(Supervisor *sup);
Name sup_entity(Supervisor *sup, const uint64_t systems);