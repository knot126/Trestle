/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the systems
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "game/gamescript.h"
#include "game/scriptman.h"
#include "global/reg.h"
#include "graphics/graphics.h"
#include "graph/graph.h"
#include "physics/physics.h"
#include "input/input.h"
#include "window/window.h"
#include "compute/vulkan.h"

enum {
	ENT_TRANSFORM = (1 << 0),
	
	ENT_GRAPHICS_MESH = (1 << 1),
	ENT_GRAPHICS_CURVE = (1 << 2),
	
	ENT_PHYSICS_OBJECT = (1 << 3),
	ENT_PHYSICS_SPHERE = (1 << 4),
	ENT_PHYSICS_AABB = (1 << 5),
	ENT_PHYSICS_MESH = (1 << 6),
	
	ENT_GRAPHICS_MESH2D = (1 << 7),
	ENT_GRAPHICS_SURFACE = (1 << 8),
};

typedef struct Supervisor {
	/**
	 * Structure containing all of the system states.
	 */
	
	GraphicsSystem graphics;
	InputSystem input;
	PhysicsSystem physics;
	Registry reg;
	SceneGraph graph;
	ScriptManager scriptman;
	
	// NOTE: These maybe probably be replaced with systems that allow creating
	// multipule of them.
	Window_t window;
	
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
