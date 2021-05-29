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
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/time.h"
#include "util/fail.h"
#include "util/xml.h"
#include "util/ini.h"
#include "util/fs.h"
#include "util/script.h"
#include "util/rand.h"
#include "util/str.h"
#include "util/log.h"
#include "graphics/graphics.h"
#include "game/gameplay.h"
#include "game/phys.h"
#include "types.h"

#include "game.h"

static void print_info(void) {
	DgLog(DG_LOG_INFO, "Engine compiled on %s at %s.", __DATE__, __TIME__);
}

static void sys_init(SystemStates *sys) {
	// Set to null
	memset(sys, 0, sizeof(SystemStates));
	
	// Graphics initialisation
	DgLog(DG_LOG_INFO, "Init graphics subsystem...");
	sys->graphics = graphics_init();
	
	// Input initialisation
	DgLog(DG_LOG_INFO, "Init input system...");
	input_init(sys->graphics);
	
	// Testing lua loader
	DgLog(DG_LOG_INFO, "Loading main game script...");
	DgScriptInit(&sys->game_script);
	DgRegisterRandFuncs(&sys->game_script);
	registerWorldScriptFunctions(&sys->game_script);
	DgScriptLoad(&sys->game_script, DgINIGet(g_quickRunConfig, "Main", "include_script_path", "assets://scripts/include.lua"));
	DgScriptLoad(&sys->game_script, DgINIGet(g_quickRunConfig, "Main", "startup_script_path", "assets://scripts/startup.lua"));
	DgScriptCall(&sys->game_script, "init");
}

static void sys_destroy(SystemStates *sys) {
	DgLog(DG_LOG_INFO, "Freeing graphics subsystem...");
	graphics_free(sys->graphics);
	
	DgLog(DG_LOG_INFO, "Freeing resources used by main script...");
	DgScriptFree(&sys->game_script);
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
		
		DgScriptCall(&systems->game_script, "tick");
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
	DgLog(DG_LOG_INFO, "Initialising file system paths...");
	DgInitPaths();
	
	// Load config
	DgLog(DG_LOG_INFO, "Loading engine configuration file...");
	
	DgINIDocument initconf;
	uint32_t initconf_status = DgINILoad(&initconf, "assets://config.ini");
	
	if (initconf_status) {
		DgFail("Error: Failed to load configuration file.\n", 200);
	}
	else {
		g_quickRunConfig = &initconf;
	}
	
	// Load world
	DgLog(DG_LOG_INFO, "Initialising main world...");
	World main_world;
	world_init(&main_world, 0);
	SetActiveWorld(&main_world);
	
	// Load systems state
	// 
	// This is only for the really big systems in the game and not for anything
	// that can basically manage itself (for example, utility functions), though
	// perhaps they should be ported to use system init as well.
	DgLog(DG_LOG_INFO, "Initialising systems...");
	SystemStates systems;
	sys_init(&systems);
	
	/**
	 * 
	 * MAIN LOOP
	 * 
	 */
	
	// Main loop
	DgLog(DG_LOG_INFO, "Starting the main loop...");
	game_loop(&main_world, &systems);
	
	/**
	 * 
	 * POST OF MAIN-LOOP (ENGINE DESTRUCTION)
	 * 
	 */
	
	// Systems destruction
	DgLog(DG_LOG_INFO, "Destroying systems...");
	sys_destroy(&systems);
	
	// World destruction
	DgLog(DG_LOG_INFO, "Destroying main world...");
	world_destroy(&main_world);
	
	// Cleanup main config file
	if (!initconf_status) {
		DgLog(DG_LOG_INFO, "Freeing memory used by config...");
		DgINIFree(&initconf);
	}
	
	return 0;
}
