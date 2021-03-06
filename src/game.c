/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Main game loop and startup functions
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "game/scriptman.h"
#include "game/scripting.h"
#include "global/supervisor.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "util/thread.h"
#include "util/alloc.h"
#include "util/time.h"
#include "util/log.h"
#include "util/args.h"
#include "util/rand.h"
#include "types.h"

// Copyright and ID strings (never used)
const char *strings[] = {
	"Copyright (C) Knot126, Trestle Engine Developers. ",
	"Powered by GPU compute, mathematics and logic !",
	"Trestle Engine (aka Quick Engine) version 0.4.0, Knot126",
	"Do your homework ! https://khanacademy.org/math "
};

DgArgs args;

typedef struct GameLoopArgs {
	bool *keep_open;
	Supervisor *systems;
} GameLoopArgs;

static void print_info(void) {
	DgLog(DG_LOG_INFO, "Engine compiled on %s at %s.", __DATE__, __TIME__);
}

static void trPause(void) {
	#if defined(__GNUC__) && defined(__x86_64__)
		__asm__ ( "pause;" );
	#endif
}

static int update_loop(Supervisor *sys) {
	/** 
	 * The main loop.
	 */
	
	double last_frame_time = 0.0f;
	
	while (sys->running) {
		double frame_time = DgTime();
		
		// Check if we should still be open
		// We do it here so the script can override this status if needed
		sys->running = get_should_keep_open(&sys->graphics);
		
		// Update subsystems
		//graphics_update(&sys->graphics, &sys->graph);
		input_update(&sys->input); /// @todo This should be called from a script (?)
		scriptman_update(&sys->scriptman, last_frame_time);
		
		// Update frame time
		frame_time = DgTime() - frame_time;
		
		// Set delta time
		last_frame_time = frame_time;
	} // while (sys->running)
	
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
	DgArgParse(&args, argc, argv);
	
	// Print help
	if (DgArgGetFlag(&args, "help")) {
		DgLog(DG_LOG_INFO, "Trestle Engine (run as %s)", argv[0]);
		DgLog(DG_LOG_INFO, "");
		DgLog(DG_LOG_INFO, "Game data");
		DgLog(DG_LOG_INFO, "\t--assets <path>         Specify a custom assets filesystem (Dir/ZIP).");
		DgLog(DG_LOG_INFO, "\t                        Note: ZIP files do not work yet.\n");
		DgLog(DG_LOG_INFO, "Help and usage");
		DgLog(DG_LOG_INFO, "\t--help                  Print this help message.");
		DgLog(DG_LOG_INFO, "");
		DgLog(DG_LOG_VERBOSE, "Will not clean up resources because we are just exiting to OS next.");
		return 0;
	}
	
	// Initialise the clock
	DgInitTime();
	
	// File system module init
	DgLog(DG_LOG_INFO, "Initialising file system paths...");
	DgInitPaths2(DgArgGetValue2(&args, "assets", "../test_project/"));
	
	// Load systems state
	// The active supervisor must be set first so the script manager can init
	// the main script.
	DgLog(DG_LOG_INFO, "SubSystem Supervisor Initialise");
	Supervisor systems;
	supervisor(&systems);
	sup_init(&systems);
	
	/**
	 * 
	 * MAIN LOOP
	 * 
	 */
	
	// Main loop
	DgLog(DG_LOG_INFO, "Start main loop");
	update_loop(&systems);
	
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
	DgArgFree(&args);
	
	DgLog(DG_LOG_INFO, "Goodbye, world!");
	
	return 0;
}


int main(int argc, char *argv[]) {
	return game_main(argc, argv);
}
