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
#include "util/json.h"
#include "util/log.h"
#include "util/args.h"
#include "util/rand.h"
#include "physics/physics.h"
#include "test.h"
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
	
	while (sup->running) {
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
	
	float show_fps = 0.0f;
	
	// Physics thread
	GameLoopArgs loopargs;
	loopargs.systems = sys;
	
	DgThread t_physics;
	
	DgThreadNew(&t_physics, &physics_loop, &loopargs);
	
	while (sys->running) {
		double frame_time = DgTime();
		
		// Check if we should still be open
		// We do it here so the script can override this status if needed
		sys->running = get_should_keep_open(&sys->graphics);
		
		// Update subsystems
		graphics_update(&sys->graphics, &sys->graph);
		input_update(&sys->input);
		scriptman_update(&sys->scriptman, g_deltaTime);
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Update global delta time
		g_deltaTime = frame_time;
		
		show_fps = show_fps + frame_time;
		if (show_fps > 1.0f) {
			DgLog(DG_LOG_VERBOSE, "Frame Time: %fms", frame_time * 1000.0f, 1.0f / frame_time);
			show_fps = 0.0f;
		}
	} // while (sys->running)
	
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
	
	// Print help
	if (DgArgGetFlag("help")) {
		DgLog(DG_LOG_INFO, "Quick Run (run as %s)", argv[0]);
		DgLog(DG_LOG_INFO, "");
		DgLog(DG_LOG_INFO, "\t--image [path1;path2]   Specify a custom assets ZIP filesystem.");
		DgLog(DG_LOG_INFO, "\t--ignore-fs             Let the game ignore unsuccessful pathfinding.");
		DgLog(DG_LOG_INFO, "\t--test-mode, -t         Run all tests.");
		DgLog(DG_LOG_INFO, "\t--help                  Print this help message.");
		DgLog(DG_LOG_INFO, "");
		DgLog(DG_LOG_VERBOSE, "Will not clean up resources because we are just exiting to OS next.");
		return 0;
	}
	
	// Do tests
	if (DgArgGetFlag("test-mode") || DgArgGetFlag("t")) {
		do_all_tests();
		DgLog(DG_LOG_VERBOSE, "Will not clean up resources because we are just exiting to OS next.");
		return 0;
	}
	
	// Initialise the clock
	DgInitTime();
	
	// File system module init
	DgLog(DG_LOG_INFO, "Initialising file system paths...");
	DgInitPaths( DgArgGetFlag("ignore-fs") ? DgLog(DG_LOG_INFO, "Filesystem errors will be ignored, the engine may behave erattically."), DG_PATH_FAIL_ERROR : DG_PATH_FAIL_FATAL );
	
	// Load systems state
	// The active supervisor must be set first so the script manager can init
	// the main script.
	DgLog(DG_LOG_INFO, "SubSystem Supervisor Initialise");
	Supervisor systems;
	supervisor(&systems);
	sup_init(&systems);
	
	// test surfaces
	DgSurface3D md;
	DgSurface3DInit(&md, 4, 4);
	
	DgSurface3DSetPoint(&md, 0, 0, &(DgVec3) {0.0, 1.2, 0.0});
	DgSurface3DSetPoint(&md, 0, 1, &(DgVec3) {1.0, 1.4, 0.0});
	DgSurface3DSetPoint(&md, 0, 2, &(DgVec3) {2.0, 1.5, 0.0});
	DgSurface3DSetPoint(&md, 0, 3, &(DgVec3) {3.0, 1.1, 0.0});
	
	DgSurface3DSetPoint(&md, 1, 0, &(DgVec3) {0.0, 1.0, 1.0});
	DgSurface3DSetPoint(&md, 1, 1, &(DgVec3) {1.0, 1.3, 1.0});
	DgSurface3DSetPoint(&md, 1, 2, &(DgVec3) {2.0, 1.2, 1.0});
	DgSurface3DSetPoint(&md, 1, 3, &(DgVec3) {3.0, 1.0, 1.0});
	
	DgSurface3DSetPoint(&md, 2, 0, &(DgVec3) {0.0, 1.6, 2.0});
	DgSurface3DSetPoint(&md, 2, 1, &(DgVec3) {1.0, 1.7, 2.0});
	DgSurface3DSetPoint(&md, 2, 2, &(DgVec3) {2.0, 1.3, 2.0});
	DgSurface3DSetPoint(&md, 2, 3, &(DgVec3) {3.0, 1.0, 2.0});
	
	DgSurface3DSetPoint(&md, 3, 0, &(DgVec3) {0.0, 1.2, 2.0});
	DgSurface3DSetPoint(&md, 3, 1, &(DgVec3) {1.0, 1.0, 2.0});
	DgSurface3DSetPoint(&md, 3, 2, &(DgVec3) {2.0, 1.6, 2.0});
	DgSurface3DSetPoint(&md, 3, 3, &(DgVec3) {3.0, 1.9, 2.0});
	
	Name surf = graphics_create_surface3d(&systems.graphics, sup_next_name(&systems));
	graph_create(&systems.graph, surf);
	graph_set(&systems.graph, surf, (Transform){.pos = (DgVec3){0.0f, 1.0f, 0.0f}, .scale = {1.0f, 1.0f, 1.0f}});
	graphics_surface3d_add_patch(&systems.graphics, surf, &md);
	
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
	
	// Cleanup arguments memory
	DgLog(DG_LOG_INFO, "Freeing memory used by argument parser...");
	DgArgFree();
	
	DgLog(DG_LOG_INFO, "Goodbye, world!");
	
	return 0;
}
