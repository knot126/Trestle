/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Main functions
 */

#include "includes.h"

#include "util/log.h"
#include "util/time.h"

#include "scene/scene.h"
#include "graphics/graphics.h"

#include "main.h"

void TrEngineInit(TrEngine *engine) {
	/**
	 * Starts the engine
	 * 
	 * @param engine The game engine context
	 */
	
	DgLog(DG_LOG_VERBOSE, "TrSceneInit");
	TrSceneInit(&engine->scene);
	
	DgLog(DG_LOG_VERBOSE, "TrGraphicsInit");
	TrGraphicsInit(&engine->graphics, &engine->scene);
}

void TrEngineTick(TrEngine *engine) {
	/**
	 * Takes care of one engine tick
	 * 
	 * @param engine The game engine context
	 */
	
	DgLog(DG_LOG_VERBOSE, "TrGraphicsUpdate");
	TrGraphicsUpdate(&engine->graphics, &engine->scene);
}

void TrEngineFree(TrEngine *engine) {
	/**
	 * Free the engine
	 * 
	 * @param engine The game engine context
	 */
	
	DgLog(DG_LOG_VERBOSE, "TrGraphicsFree");
	TrGraphicsFree(&engine->graphics, &engine->scene);
}

bool TrEngineIsRunning(TrEngine *engine) {
	/**
	 * Return if the engine should keep running or exit
	 * 
	 * @param engine The game engine context
	 * @return 1 if the engine is still running, zero if not
	 */
	
	return engine->scene.running; // TEMP
}

void TrInitGlobals(void) {
	/**
	 * Initialise global state for Melon
	 */
	
	DgInitTime();
}

int main(const int argc, const char *argv[]) {
	/**
	 * The main function.
	 * 
	 * @param argc Number of arguments passed to the program
	 * @param argv The arguments (as array of strings)
	 * @return Exit code
	 */
	
	// Initialise melon library
	TrInitGlobals();
	
	// Main engine init
	TrEngine engine;
	
	DgLog(DG_LOG_VERBOSE, "TrEngineInit");
	TrEngineInit(&engine);
	
	// Main loop
	while (TrEngineIsRunning(&engine)) {
		double start = DgTime();
		
		DgLog(DG_LOG_VERBOSE, "TrEngineTick");
		TrEngineTick(&engine);
		
		double delta = DgTime() - start;
		
		DgLog(DG_LOG_INFO, ":: Frame time %.4fms (%.2f fps) ::", delta * 1000.0f, 1.0f / delta);
	}
	
	// Free the engine
	DgLog(DG_LOG_VERBOSE, "TrEngineFree");
	TrEngineFree(&engine);
	
	return 0;
}
