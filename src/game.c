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
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/time.h"
#include "util/xml.h"
#include "util/ini.h"
#include "util/fs.h"
#include "util/script.h"
#include "util/log.h"
#include "util/args.h"
#include "graphics/graphics.h"
#include "game/gameplay.h"
#include "game/phys.h"
#include "game/level.h"
#include "game/gamescript.h"
#include "game/scripting.h"
#include "types.h"

#include "game.h"

#define QR_EXPRIMENTAL_THREADING

typedef struct GameLoopArgs {
	bool *keep_open;
	World *world;
	SystemStates *systems;
} GameLoopArgs;

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
	
	// Run the main game script
	DgLog(DG_LOG_INFO, "Loading main game script...");
	game_script_new(&sys->game_script);
	game_script_yeild(&sys->game_script, DgINIGet(g_quickRunConfig, "Main", "startup_script_path", "assets://scripts/startup.lua"));
	game_script_active(&sys->game_script);
	
	// Init the level manager
	DgLog(DG_LOG_INFO, "Initialising level manager...");
	level_init(&sys->level_info, DgINIGet(g_quickRunConfig, "Resources", "level_index_path", "assets://game.xml"));
}

static void sys_destroy(SystemStates *sys) {
	DgLog(DG_LOG_INFO, "Freeing graphics subsystem...");
	graphics_free(sys->graphics);
	
	DgLog(DG_LOG_INFO, "Freeing resources used by main script...");
	game_script_free(&sys->game_script);
	
	DgLog(DG_LOG_INFO, "Freeing resources used by level manager...");
	level_free(&sys->level_info);
}

#ifdef QR_EXPRIMENTAL_THREADING
static void *physics_loop(void *args_) {
	/**
	 * The loop that updates the physics system.
	 */
	GameLoopArgs *args = (GameLoopArgs *) args_;
	World *world = args->world;
	SystemStates *systems = args->systems;
	
	double accumulate = 0.0f;
	double show_time = 0.0f;
	
	while (*args->keep_open) {
		float frame_time = (float) DgTime();
		
		if (accumulate > g_physicsDelta) {
			float time = DgTime();
			
			phys_update(world, g_physicsDelta);
			accumulate = 0.0f;
			
			time = DgTime() - time;
			
			if (show_time > 1.0f) {
				DgLog(DG_LOG_VERBOSE, "Physics frame time: %fms", time);
				show_time = 0.0f;
			}
		}
		
		frame_time = DgTime() - frame_time;
		
		accumulate += frame_time;
		show_time += frame_time;
		
#if defined(__GNUC__) && defined(__x86_64__)
		__asm__ ( "pause;" );
#endif
	}
}
#endif

static int game_loop(World *world, SystemStates *systems) {
	/* 
	 * The main loop.
	 */
	bool should_keep_open = true;
	
	float show_fps = 0.0f;
	
	// We will accumulate and update physics when time is right.
#ifndef QR_EXPRIMENTAL_THREADING
	float accumulate = 0.0f;
#endif
	
#ifdef QR_EXPRIMENTAL_THREADING
	GameLoopArgs loopargs;
	loopargs.world = world;
	loopargs.systems = systems;
	loopargs.keep_open = &should_keep_open;
	DgThread thrd;
	
	DgThreadNew(&thrd, &physics_loop, &loopargs);
#endif
	
	while (should_keep_open) {
		float frame_time = DgTime();
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(systems->graphics);
		
		graphics_update(world, systems->graphics);
		input_update(systems->graphics);
		
		game_script_update(&systems->game_script);
		
#ifndef QR_EXPRIMENTAL_THREADING
		if (accumulate > g_physicsDelta) {
			phys_update(world, g_physicsDelta);
			accumulate = 0.0f;
		}
#endif
		
		if (!world_get_pause(world)) {
			gameplay_update(world);
		}
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Update global delta time
		g_deltaTime = frame_time;
		
		// Update accumulator
#ifndef QR_EXPRIMENTAL_THREADING
		accumulate += frame_time;
#endif
		
		show_fps = show_fps + frame_time;
		if (show_fps > 1.0f) {
			DgLog(DG_LOG_VERBOSE, "Frame Time: %fms (%f FPS)", frame_time * 1000.0f, 1.0f / frame_time);
			show_fps = 0.0f;
		}
		
		// Update the level
		if (!world_get_pause(world)) {
			level_update(world, &systems->level_info);
		}
	} // while (should_keep_open)
	
#ifdef QR_EXPRIMENTAL_THREADING
	DgThreadJoin(&thrd);
#endif
	
	return 0;
}

int game_main(int argc, char* argv[]) {
	/**
	 * The first real main game function, called from the main() function of the
	 * OS. 
	 */
	print_info();
	
	// Parse command line arguments
	DgLog(DG_LOG_INFO, "Parsing command line arguments...");
	DgArgParse(argc, argv);
	
	// Initialise the clock
	DgInitTime();
	
	// File system module init
	DgLog(DG_LOG_INFO, "Initialising file system paths...");
	DgInitPaths();
	
	// Print help
	if (DgArgGetFlag("help")) {
		DgLog(DG_LOG_INFO, "Quick Run (run as %s)", argv[0]);
		DgLog(DG_LOG_INFO, "");
		DgLog(DG_LOG_INFO, "\t--config [path]         Specify a custom engine config file to load.");
		DgLog(DG_LOG_INFO, "\t--assets [path1;path2]  Specify a custom assets ZIP filesystem.");
		DgLog(DG_LOG_INFO, "\t--help                  Print this help message.");
		DgLog(DG_LOG_INFO, "");
		return 0;
	}
	
	// Load config
	DgLog(DG_LOG_INFO, "Loading engine configuration file...");
	
	DgINIDocument initconf;
	char *config = DgArgGetValue2("config", "assets://config.ini");
	uint32_t initconf_status = DgINILoad(&initconf, config);
	
	if (initconf_status) {
		DgLog(DG_LOG_FATAL, "Failed to load configuration file at '%s'.", config);
		return 1;
	}
	else {
		g_quickRunConfig = &initconf;
	}
	
	// testing audio
// 	DgAudioStream audio;
// 	DgAudioStreamNew(&audio);
// 	
// 	float *audio_data = (float *) DgAlloc(sizeof *audio_data * 88200);
// 	
// 	if (audio_data) {
// 		for (size_t i = 0; i < 44100; i++) {
// 			float s = DgSin(i * (1.0f / (44100.0f / 440.0f)));
// 			audio_data[i * 2] = s;
// 			audio_data[i * 2 + 1] = s;
// 		}
// 		
// 		DgAudioStreamPush(&audio, sizeof *audio_data * 88200, audio_data);
// 		
// 		DgFree(audio_data);
// 		DgAudioStreamFree(&audio);
// 	}
	
	// Load world
	DgLog(DG_LOG_INFO, "Initialising main world...");
	World main_world;
	world_init(&main_world, 0);
	world_active(&main_world);
	
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
	
	// Cleanup arguments memory
	DgLog(DG_LOG_INFO, "Freeing memory used by argument parser...");
	DgArgFree();
	
	return 0;
}
