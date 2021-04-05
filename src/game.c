/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file is part of the Decent Games Engine. Please do not redistribute the
 * sources without prior permission from Decent Games and/or contributours.
 * 
 * Contributours:
 *  - cdde <cddepppp256@protonmail.com>
 * 
 * Main game loop and startup functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#if defined(__linux__)
	#include <unistd.h>
#endif

#include "generic/world.h"
#include "generic/transform.h"
#include "graphics/graphics.h"
#include "graphics/components.h"
#include "phys/phys.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/bag.h"
#include "util/flag.h"
#include "util/time.h"
#include "util/fail.h"
#include "util/xml.h"
#include "io/fs.h"
#include "io/config.h"
#include "io/input.h"
#include "systems.h"
#include "types.h"

static void print_info(void) {
	printf("Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

static int game_loop(World *world, SystemStates *systems) {
	/* 
	 * The main loop.
	 */
	bool should_keep_open = true;
	
	float show_fps = 0.0;
	
	// We will accumulate and update physics when time is right.
	float accumulate = 0.0f;
	const float phys_delta_time = 1.0f / 180.0f;
	
	while (should_keep_open) {
		float frame_time = DgTime();
		float clock; // time var for clocker
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(systems->graphics);
		
		//DgThread t_graphics, t_physics;
		
		graphics_update(world, systems->graphics);
		input_update(systems->graphics);
		
		if (accumulate > phys_delta_time) {
			phys_update();
			accumulate = 0.0f;
		}
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Update global delta time
		g_deltaTime = frame_time;
		
		// Update accumulator
		accumulate += frame_time;
		
		show_fps = show_fps + frame_time;
		if (show_fps > 1.0f) {
			printf("FPS: %d (%fms)\n", (int) (1 / frame_time), frame_time * 1000.0f);
			show_fps = 0.0f;
		}
	} // while (should_keep_open)
	
	return 0;
}

static void on_init_okay(const char* event, void* params) {
	printf("The game has initialised successfully.\n");
}

int game_main(int argc, char* argv[]) {
	/* The first real main game function, called from the main() function of the
	 * OS. */
	print_info();
	
	// Initialise the clock
	DgInitTime();
	
	// File system module init
	printf("Info: Initialising file system paths...\n");
	DgInitPaths();
	
	// Load config
	printf("Info: Loading engine configuration file...\n");
	DgBag *config = DgConfigLoad("assets://config.cfg", true);
	
	if (!config) {
		DgFail("Error: Failed to load configuration file.\n", 1);
	}
	
	DgBagPrint(config);
	
	// Loading XML config
	DgXMLNode settings_doc;
	DgXMLLoad(&settings_doc, "assets://config.xml");
	
	// Event centre startup (global events)
	DgFlagCreateEvent("game_init_ok");
	DgFlagRegisterCallback("game_init_ok", &on_init_okay);
	
	// Load world
	printf("Info: Initialising main world...\n");
	World main_world;
	world_init(&main_world, 0);
	
	// Create test entity
	uint32_t ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH);
	entity_load_mesh(&main_world, ent, "assets://mesh/cube2.bin");
	
	ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH);
	entity_set_transform(&main_world, ent, DgVec3New(-1.25f, 0.0f, 0.0f), DgVec3New(0.0f, 0.0f, 0.0f));
	entity_load_mesh(&main_world, ent, "assets://mesh/cube3.bin");
	
	// Load systems state
	// 
	// This is only for the really big systems in the game and not for anything
	// that can basically manage itself (for example, utility functions), though
	// perhaps they should be ported to use system init as well.
	printf("Info: Initialising systems...\n");
	SystemStates systems;
	sys_init(&systems);
	
	// Main loop
	printf("Info: Starting the main loop...\n");
	DgFlagRaise("game_init_ok", NULL);
	game_loop(&main_world, &systems);
	
	// Systems destruction
	printf("Info: Destroying systems...\n");
	sys_destroy(&systems);
	
	// Global flags cleanup
	printf("Info: Cleaning up memory used by flags...\n");
	DgFlagGlobalCleanup();
	
	// Cleanup main config file
	if (config) {
		printf("Info: Freeing memory used by config...\n");
		DgConfigFree(config);
	}
	
	return 0;
}
