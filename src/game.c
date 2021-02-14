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

//#include "graphics/vulkan.h"
//#include "graphics/opengl.h"
#include "graphics/graphics.h"
#include "phys/phys.h"
#include "util/alloc.h"
#include "util/bag.h"
#include "util/flag.h"
#include "util/time.h"
#include "io/fs.h"

const float phys_delta_time = 1.0f / 180.0f;
static DgBag g_gameProperties;

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
	// We will accumulate and update physics when time is right.
	float accumulate = 0.0f;
	
	while (should_keep_open) {
		float frame_time = DgTime();
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(graphics_info);
		
		graphics_update(graphics_info);
		
		if (accumulate > phys_delta_time) {
			phys_update();
			accumulate = 0.0f;
		}
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Update accumulator
		accumulate += frame_time;
		
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
	DgAllocPrintChain();
	
	// File system module init
	printf("Initialising file system paths...\n");
	DgInitPaths();
	
	// Create a global property bag
	printf("Preparing global properties...\n");
	g_gameProperties = DgBagInit();
	DgBagSet(&g_gameProperties, "deploy", "0");
	DgBagSet(&g_gameProperties, "graphicsDriver", "OpenGL");
	
	const char* test = DgBagGet(&g_gameProperties, "deploy");
	printf("deploy = %s\n", test);
	
	DgBagPrint(&g_gameProperties);
	
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
