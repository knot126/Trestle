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

#include "world/world.h"
#include "world/transform.h"
#include "world/compo/graphics.h"
#include "graphics/graphics.h"
#include "phys/phys.h"
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/bag.h"
#include "util/flag.h"
#include "util/time.h"
#include "util/fail.h"
#include "util/xml.h"
#include "util/fs.h"
#include "util/config.h"
#include "util/rand.h"
#include "gameplay/gameplay.h"
#include "types.h"

#include "game.h"

static void print_info(void) {
	printf("Info: Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

static void on_init_okay(const char* event, void* params) {
	printf("Info: The game has initialised successfully.\n");
}

static void sys_init(SystemStates *sys) {
	// Set to null
	memset(sys, 0, sizeof(SystemStates));
	
	// Graphics initialisation
	printf("Info: Init graphics subsystem...\n");
	sys->graphics = graphics_init();
	
	// Input initialisation
	printf("Info: Init input system...\n");
	input_init(sys->graphics);
}

static void sys_destroy(SystemStates *sys) {
	printf("Info: Destroying graphics subsystem...\n");
	graphics_free(sys->graphics);
}

static int game_loop(World *world, SystemStates *systems) {
	/* 
	 * The main loop.
	 */
	bool should_keep_open = true;
	
	float show_fps = 0.0;
	
	// We will accumulate and update physics when time is right.
	float accumulate = 0.0f;
	
	while (should_keep_open) {
		float frame_time = DgTime();
		float clock; // time var for clocker
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(systems->graphics);
		
		DgThread th_graphics, th_physics;
		
		graphics_update(world, systems->graphics);
		input_update(systems->graphics);
		
		if (accumulate > g_physicsDelta) {
			phys_update(world, g_physicsDelta);
			accumulate = 0.0f;
		}
		
		gameplay_update(world);
		
		//DgThreadJoin(&th_graphics);
		
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
		DgFail("Error: Failed to load configuration file.\n", 200);
	}
	
	// Loading XML config
	printf("Info: Loading settings document (XML version).\n");
	DgXMLNode settings_doc;
	uint32_t status = DgXMLLoad(&settings_doc, "assets://config.xml");
	if (!status) {
		DgXMLFree(&settings_doc);
	}
	else {
		printf("Error: Loading settings document failed (XML version).\n");
	}
	
	// Event centre startup (global events)
	DgFlagCreateEvent("game_init_ok");
	DgFlagRegisterCallback("game_init_ok", &on_init_okay);
	
	// Load world
	printf("Info: Initialising main world...\n");
	World main_world;
	world_init(&main_world, 0);
	
	// Create test entities
	uint32_t ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH);
	entity_load_mesh(&main_world, ent, "assets://mesh/cube2.bin");
	entity_set_transform(&main_world, ent, DgVec3New(0.0f, 0.0f, 0.2f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(3.0f, 0.1f, 8.0f));
	
	ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH | QR_COMPONENT_PHYSICS);
	entity_set_transform(&main_world, ent, DgVec3New(-1.25f, 0.0f, 0.0f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(1.0f, 1.5f, 1.0f));
	entity_set_physics(&main_world, ent, DgVec3New(0.0f, 0.0f, -0.5f), DgVec3New(0.0f, 0.0f, -0.1f), DgVec3New(1.0f, 1.0f, 1.0f));
	entity_load_mesh(&main_world, ent, "assets://mesh/cube3.bin");
	
	ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_CAMERA | QR_COMPONENT_PHYSICS);
	entity_set_transform(&main_world, ent, DgVec3New(0.0f, 2.0f, 3.0f), DgVec3New(0.1f, 0.0f, 0.0f), DgVec3New(1.0f, 1.0f, 1.0f));
	entity_set_physics(&main_world, ent, DgVec3New(0.0f, 0.0f, -0.25f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(1.0f, 1.0f, 1.0f));
	world_set_camera(&main_world, ent);
	
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
	
	// World destruction
	printf("Info: Destroying main world...\n");
	world_destroy(&main_world);
	
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
