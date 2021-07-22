/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the suptems
 */ 

#include <string.h>
#include <inttypes.h>

#include "game/gamescript.h"
#include "graph/graph.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "util/log.h"
#include "util/ini.h"
#include "physics/physics.h"
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
	
	// Graphics initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Graphics");
	graphics_init(&sup->graphics);
	graphics(&sup->graphics);
	
	// Physics initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Physics");
	physics_init(&sup->physics);
	
	// Scene graph initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Scene Graph");
	graph_init(&sup->graph);
	
	// Input initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Input");
	input_init(&sup->input, &sup->graphics);
	
	// Run the main game script
	DgLog(DG_LOG_INFO, "Supervisor Initialise: GameScript");
	game_script_new(&sup->game_script);
	game_script_yeild(&sup->game_script, DgINIGet(g_quickRunConfig, "Main", "startup_script_path", "assets://scripts/startup.lua"));
	game_script_active(&sup->game_script);
}

void sup_destroy(Supervisor * restrict sup) {
	/**
	 * Destroy all the systems in the game.
	 */
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: GameScript");
	game_script_free(&sup->game_script);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Input");
	// Not currently required
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Scene Graph");
	graph_free(&sup->graph);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Physics");
	physics_free(&sup->physics);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Graphics");
	graphics_free(&sup->graphics);
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
	
	if ((systems & ENT_GRAPHICS_CURVE) == ENT_GRAPHICS_CURVE) {
		// ...
	}
	
	if ((systems & ENT_PHYSICS_OBJECT) == ENT_PHYSICS_OBJECT) {
		physics_create_object(&sup->physics, name);
	}
	
	if ((systems & ENT_PHYSICS_AABB) == ENT_PHYSICS_AABB) {
		physics_create_aabb(&sup->physics, name);
	}
	
	return name;
}
