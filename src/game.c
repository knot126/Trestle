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
#include "graphics/graphics.h"
#include "phys/phys.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/clocker.h"
#include "util/bag.h"
#include "util/flag.h"
#include "util/time.h"
#include "util/fail.h"
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
		
		graphics_update(systems->graphics);
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
			printf("FPS: %d (%fms)\n", (int) (1 / frame_time), frame_time);
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
	
	// Create a basic memory pool
	printf("Making the initial memory pool...\n");
	alloch_t mempool = DgAllocPoolInit(1024 * 1024 * 8);
	
	// File system module init
	printf("Initialising file system paths...\n");
	DgInitPaths();
	
	// Load config
	printf("Loading engine configuration file...\n");
	DgConfig *config = DgConfigLoad("assets://config.ini", true);
	
	if (!config) {
		DgFail("Error: Failed to load configuration file.\n", 1);
	}
	
	// Event centre startup (global events)
	DgFlagCreateEvent("game_init_ok");
	DgFlagRegisterCallback("game_init_ok", &on_init_okay);
	
	// Load world
	printf("Initialising main world...\n");
	World main_world;
	world_init(&main_world, 0);
	
	// Load systems state
	// 
	// This is only for the really big systems in the game and not for anything
	// that can basically manage itself (for example, utility functions), though
	// perhaps they should be ported to use system init as well.
	printf("Initialising systems...\n");
	SystemStates systems;
	sys_init(&systems);
	
	// Main loop
	printf("Starting the main loop...\n");
	DgFlagRaise("game_init_ok", NULL);
	game_loop(&main_world, &systems);
	
	// Systems destruction
	printf("Destroying systems...\n");
	sys_destroy(&systems);
	
	// Global flags cleanup
	printf("Cleaning up memory used by flags...\n");
	DgFlagGlobalCleanup();
	
	// Cleanup main config file
	if (config) {
		printf("Freeing memory used by config...\n");
		DgConfigFree(config);
	}
	
	// Free pool
	printf("Free memory pool...\n");
	DgAllocPoolFree(mempool);
	
	return 0;
}
