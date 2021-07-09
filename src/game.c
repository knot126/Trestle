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

#include "game/gameplay.h"
#include "game/phys.h"
#include "game/level.h"
#include "game/gamescript.h"
#include "game/scripting.h"
#include "global/supervisor.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/time.h"
#include "util/ini.h"
#include "util/log.h"
#include "util/args.h"
#include "physics/physics.h"
#include "world/world.h"
#include "types.h"

#include "game.h"

// #define QR_EXPRIMENTAL_THREADING

typedef struct GameLoopArgs {
	bool *keep_open;
	World *world;
	Supervisor *systems;
} GameLoopArgs;

static void print_info(void) {
	DgLog(DG_LOG_INFO, "Engine compiled on %s at %s.", __DATE__, __TIME__);
}

static void *physics_loop(void *args_) {
	/**
	 * The loop that updates the physics system.
	 */
	GameLoopArgs *args = (GameLoopArgs *) args_;
	World *world = args->world;
	Supervisor *systems = args->systems;
	
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

#ifdef QR_EXPRIMENTAL_THREADING
static void *graphics_loop(void *args_) {
	GameLoopArgs *args = (GameLoopArgs *) args_;
	
	World *world = args->world;
	Supervisor *sys = args->systems;
	
	float show_time = 0.0f;
	
	while (*args->keep_open) {
		float time = DgTime();
		
		graphics_update(&sys->graphics);
		
		time = DgTime() - time;
		
		if (show_time > 1.0f) {
			DgLog(DG_LOG_VERBOSE, "Graphics frame time: %fms", time);
			show_time = 0.0f;
		}
		
		show_time += time;
	}
}
#endif

static int game_loop(World *world, Supervisor *sys) {
	/** 
	 * The main loop.
	 */
	
	bool should_keep_open = true;
	
	float show_fps = 0.0f;
	
	// Physics thread
	GameLoopArgs loopargs;
	loopargs.world = world;
	loopargs.systems = sys;
	loopargs.keep_open = &should_keep_open;
	
	DgThread t_physics;
#ifdef QR_EXPRIMENTAL_THREADING
	DgThread t_graphics;
#endif
	
	DgThreadNew(&t_physics, &physics_loop, &loopargs);
#ifdef QR_EXPRIMENTAL_THREADING
	DgThreadNew(&t_graphics, &graphics_loop, &loopargs);
#endif
	
	while (should_keep_open) {
		double frame_time = DgTime();
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(&sys->graphics);
		
#ifndef QR_EXPRIMENTAL_THREADING
		graphics_update(&sys->graphics);
#endif
		input_update(&sys->input);
		
		game_script_update(&sys->game_script);
		
		if (!world_get_pause(world)) {
			// NOTE: This function should not exist because all of this stuff 
			// should be scripted.
			gameplay_update(world, &sys->graphics);
		}
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Update global delta time
		g_deltaTime = frame_time;
		
		show_fps = show_fps + frame_time;
		if (show_fps > 1.0f) {
			DgLog(DG_LOG_VERBOSE, "Frame Time: %fms", frame_time * 1000.0f, 1.0f / frame_time);
			show_fps = 0.0f;
		}
		
		// Update the level
		if (!world_get_pause(world)) {
			level_update(world, &sys->level_info);
		}
	} // while (should_keep_open)
	
	DgThreadJoin(&t_physics);
#ifdef QR_EXPRIMENTAL_THREADING
	DgThreadJoin(&t_graphics);
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
	char *config = (char *) DgArgGetValue2("config", "assets://config.ini");
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
	Supervisor systems;
	sup_init(&systems, &main_world);
	
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
	sup_destroy(&systems);
	
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
