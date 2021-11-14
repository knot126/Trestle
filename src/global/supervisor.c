/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the suptems
 */ 

#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "game/gamescript.h"
#include "game/scriptman.h"
#include "global/reg.h"
#include "graph/graph.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "util/log.h"
#include "physics/physics.h"
#include "window/window.h"
#include "compute/vulkan.h"
#include "types.h"

#include "supervisor.h"

Supervisor *QR_MAIN_SUPERVISOR;

Supervisor *supervisor(Supervisor *S) {
	/**
	 * Get and set the main supervisor.
	 */
	
	if (S) {
		QR_MAIN_SUPERVISOR = S;
	}
	
	return QR_MAIN_SUPERVISOR;
}

void sup_init(Supervisor * restrict sup) {
	/**
	 * Initialise all the systems in the game.
	 */
	
	// Set to null
	memset(sup, 0, sizeof(Supervisor));
	sup->next = 1;
	
	// Window initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Window");
	window_init(&sup->window);
	
	// Vulkan initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Compute");
	DgLog(DG_LOG_WARNING, "todo: implement compute in supervisor");
	
	// Graphics initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Graphics");
	graphics_init(&sup->graphics);
	
	// Physics initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Physics");
	physics_init(&sup->physics);
	
	// Scene graph initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Scene Graph");
	graph_init(&sup->graph);
	
	// Input initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Input");
	input_init(&sup->input, &sup->graphics);
	
	// Initialise the registry
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Registry");
	registry_init(&sup->reg);
	
	// Initialise the script manager
	DgLog(DG_LOG_INFO, "Supervisor Initialise: ScriptManager");
	scriptman_init(&sup->scriptman);
	
	// Run the main game script
	scriptman_open(&sup->scriptman, "assets://init.lua");
	
	sup->running = true;
}

void sup_destroy(Supervisor * restrict sup) {
	/**
	 * Destroy all the systems in the game.
	 */
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: ScriptManager");
	scriptman_free(&sup->scriptman);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Registry");
	registry_free(&sup->reg);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Input");
	// Not currently required
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Scene Graph");
	graph_free(&sup->graph);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Physics");
	physics_free(&sup->physics);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Graphics");
	graphics_free(&sup->graphics);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Compute");
	DgLog(DG_LOG_WARNING, "todo: implement compute in supervisor");
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Window");
	window_free(&sup->window);
}

void sup_close(Supervisor *sup) {
	/**
	 * Set the game as not running anymore.
	 */
	
	sup->running = false;
}

Name sup_next_name(Supervisor *sup) {
	/**
	 * Get the next name.
	 */
	
	return sup->next++;
}

Name sup_entity(Supervisor *sup, const uint64_t systems) {
	/**
	 * Make an entity in the requested systems.
	 */
	
	Name name = sup_next_name(sup);
	
	if ((systems & ENT_TRANSFORM) == ENT_TRANSFORM) {
		graph_create(&sup->graph, name);
	}
	
	if ((systems & ENT_GRAPHICS_MESH) == ENT_GRAPHICS_MESH) {
		graphics_create_mesh(&sup->graphics, name);
	}
	
	if ((systems & ENT_GRAPHICS_MESH2D) == ENT_GRAPHICS_MESH2D) {
		graphics_create_mesh2d(&sup->graphics, name);
	}
	
	if ((systems & ENT_GRAPHICS_CURVE) == ENT_GRAPHICS_CURVE) {
		// ...
	}
	
	if ((systems & ENT_PHYSICS_OBJECT) == ENT_PHYSICS_OBJECT) {
		physics_create_object(&sup->physics, name);
	}
	
	if ((systems & ENT_PHYSICS_AABB) == ENT_PHYSICS_AABB) {
		physics_create_aabb(&sup->physics, name);
	}
	
	if ((systems & ENT_GRAPHICS_SURFACE) == ENT_GRAPHICS_SURFACE) {
		graphics_create_surface3d(&sup->graphics, name);
	}
	
	return name;
}
