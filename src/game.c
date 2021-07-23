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

#include "game/gamescript.h"
#include "game/scripting.h"
#include "global/supervisor.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/time.h"
#include "util/ini.h"
#include "util/json.h"
#include "util/log.h"
#include "util/args.h"
#include "physics/physics.h"
#include "types.h"

#include "game.h"

// #define QR_EXPRIMENTAL_THREADING

typedef struct GameLoopArgs {
	bool *keep_open;
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
	Supervisor *sup = args->systems;
	
	double accumulate = 0.0f;
	double show_time = 0.0f;
	
	while (*args->keep_open) {
		double frame_time = DgTime();
		
		if (accumulate >= g_physicsDelta) {
			float time = DgTime();
			
			physics_update(&sup->physics, &sup->graph, g_physicsDelta);
			
			time = DgTime() - time;
			
			if (show_time > 1.0f) {
				DgLog(DG_LOG_VERBOSE, "Physics frame time: %fms", time);
				show_time = 0.0f;
			}
			
			accumulate = 0.0f;
		}
		
#if defined(__GNUC__) && defined(__x86_64__)
		__asm__ ( "pause;" );
#endif
		
		frame_time = DgTime() - frame_time;
		
		accumulate += frame_time;
		show_time += frame_time;
	}
}

static int game_loop(Supervisor *sys) {
	/** 
	 * The main loop.
	 */
	
	bool should_keep_open = true;
	
	float show_fps = 0.0f;
	
	// Physics thread
	GameLoopArgs loopargs;
	loopargs.systems = sys;
	loopargs.keep_open = &should_keep_open;
	
	DgThread t_physics;
	
	DgThreadNew(&t_physics, &physics_loop, &loopargs);
	
	while (should_keep_open) {
		double frame_time = DgTime();
		
		// Check if we should still be open
		should_keep_open = get_should_keep_open(&sys->graphics);
		
		// Update subsystems
		graphics_update(&sys->graphics, &sys->graph);
		input_update(&sys->input);
		game_script_update(&sys->game_script);
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Update global delta time
		g_deltaTime = frame_time;
		
		show_fps = show_fps + frame_time;
		if (show_fps > 1.0f) {
			DgLog(DG_LOG_VERBOSE, "Frame Time: %fms", frame_time * 1000.0f, 1.0f / frame_time);
			show_fps = 0.0f;
		}
	} // while (should_keep_open)
	
	DgThreadJoin(&t_physics);
	
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
	
	DgLog(DG_LOG_INFO, "JSON test...");
	DgJSONValue val;
	const char a[] = "{\"test\": 6.30,\n\t\"abc\": \"def\", \"q\": [0, 0, 0],\n\t\"np\": null,\n\t\"a-true\": true, \"a-false\": false\n, \"objec\": {}, \"arre\": [], \"thing\": {\"ssss\": 534}}";
	
	if (DgJSONParse(&val, sizeof a - 1, a)) {
		DgLog(DG_LOG_ERROR, "Failed to load JSON document.");
	}
	
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
	
	// Load systems state
	DgLog(DG_LOG_INFO, "SubSystem Supervisor Initialise");
	Supervisor systems;
	sup_init(&systems);
	supervisor(&systems);
	
	/**
	 * 
	 * MAIN LOOP
	 * 
	 */
	
	// Main loop
	DgLog(DG_LOG_INFO, "Start main loop");
	game_loop(&systems);
	
	/**
	 * 
	 * POST OF MAIN-LOOP (ENGINE DESTRUCTION)
	 * 
	 */
	
	// Systems destruction
	DgLog(DG_LOG_INFO, "SubSystem Supervisor Destroy");
	sup_destroy(&systems);
	
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
