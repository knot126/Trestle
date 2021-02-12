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
#if defined(__linux__)
	#include <unistd.h>
#endif

//#include "graphics/vulkan.h"
//#include "graphics/opengl.h"
#include "graphics/graphics.h"
#include "util/alloc.h"
#include "util/bag.h"
#include "util/flag.h"
#include "util/time.h"
#include "io/fs.h"

static void print_info(void) {
	printf("Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

static int game_loop(GraphicsInitInfo graphics_info) {
	/* 
	 * The main loop.
	 */
	bool should_keep_open = true;
	
	if (!should_keep_open) {
		printf("Graphics pointer is null.\n");
	}
	
	float show_fps = 0.0;
	
	while (should_keep_open) {
		float frame_time = DgTime();
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(graphics_info);
		
		graphics_update(graphics_info);
		
		frame_time = DgTime() - frame_time;
		show_fps = show_fps + frame_time;
		
		if (show_fps > 1.0f) {
			printf("FPS: %d\n", (int) (1 / frame_time));
			show_fps = 0.0f;
		}
	} // while (should_keep_open)
	
	return 0;
}

// testing events
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
	
	// Event centre startup (global events)
	DgFlagCreateEvent("game_init_ok");
	DgFlagRegisterCallback("game_init_ok", &on_init_okay);
	
	// Graphics initialisation
	printf("Init graphics subsystem...\n");
	GraphicsInitInfo graphics_info = graphics_init(DG_GRAPHICS_TYPE_OPENGL);
	
	// Main loop
	printf("Starting the main loop...\n");
	DgFlagRaise("game_init_ok", NULL);
	game_loop(graphics_info);
	
	// Graphics destruction
	printf("Destroying graphics subsystem...\n");
	graphics_free(graphics_info);
	
	// Global flags cleanup
	printf("Cleaning up memory used by flags...\n");
	DgFlagGlobalCleanup();
	
	// Free pool
	printf("Free memory pool...\n");
	DgAllocPoolFree(mempool);
	
	return 0;
}
