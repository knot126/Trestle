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
#include <stdbool.h>
#include <string.h>

#include "world/world.h"
#include "world/scripting.h"
#include "graphics/graphics.h"
#include "phys/phys.h"
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/bag.h"
#include "util/time.h"
#include "util/fail.h"
#include "util/xml.h"
#include "util/ini.h"
#include "util/fs.h"
#include "util/script.h"
#include "util/config.h"
#include "util/rand.h"
#include "game/gameplay.h"
#include "types.h"

#include "game.h"

static void print_info(void) {
	printf("Info: Engine compiled on %s at %s.\n", __DATE__, __TIME__);
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
	/**
	 * The first real main game function, called from the main() function of the
	 * OS. 
	 */
	print_info();
	
	// Initialise the clock
	DgInitTime();
	
	// File system module init
	printf("\033[0;35mInfo:\033[0m Initialising file system paths...\n");
	DgInitPaths();
	
	// Load config
	printf("\033[0;35mInfo:\033[0m Loading engine configuration file...\n");
	
	DgINIDocument initconf;
	uint32_t initconf_status = DgINILoad(&initconf, "assets://config.ini");
	
	if (initconf_status) {
		DgFail("Error: Failed to load configuration file.\n", 200);
	}
	else {
		g_quickRunConfig = &initconf;
	}
	
	// Loading XML config
	DgXMLNode somedoc;
	
	if (DgXMLLoad(&somedoc, "assets://config.xml")) {
		printf("\033[1;31mError:\033[0m Failed to load XML doucment.\n");
	}
	
	DgXMLNodeFree(&somedoc);
	
	// Load world
	printf("\033[0;35mInfo:\033[0m Initialising main world...\n");
	World main_world;
	world_init(&main_world, 0);
	SetActiveWorld(&main_world);
	
	// Testing lua loader
	printf("\033[0;35mInfo:\033[0m Running startup script...\n");
	DgScript script;
	DgScriptInit(&script);
	DgRegisterRandFuncs(&script);
	registerWorldScriptFunctions(&script);
	DgScriptLoad(&script, "assets://scripts/startup.lua");
	DgScriptFree(&script);
	
	// Load systems state
	// 
	// This is only for the really big systems in the game and not for anything
	// that can basically manage itself (for example, utility functions), though
	// perhaps they should be ported to use system init as well.
	printf("\033[0;35mInfo:\033[0m Initialising systems...\n");
	SystemStates systems;
	sys_init(&systems);
	
	/**
	 * 
	 * MAIN LOOP
	 * 
	 */
	
	// Main loop
	printf("\033[0;35mInfo:\033[0m Starting the main loop...\n");
	game_loop(&main_world, &systems);
	
	/**
	 * 
	 * POST OF MAIN-LOOP (ENGINE DESTRUCTION)
	 * 
	 */
	
	// Systems destruction
	printf("\033[0;35mInfo:\033[0m Destroying systems...\n");
	sys_destroy(&systems);
	
	// World destruction
	printf("\033[0;35mInfo:\033[0m Destroying main world...\n");
	world_destroy(&main_world);
	
	// Cleanup main config file
	if (!initconf_status) {
		printf("\033[0;35mInfo:\033[0m Freeing memory used by config...\n");
		DgINIFree(&initconf);
	}
	
	return 0;
}
