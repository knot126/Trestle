/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Main functions
 */

#include "includes.h"

#include "util/log.h"

#include "scene/scene.h"
#include "graphics/graphics.h"

#include "main.h"

void TrEngineInit(TrEngine *engine) {
	/**
	 * Starts the engine
	 * 
	 * @param engine The game engine context
	 */
	
	TrGraphicsInit(&engine->graphics, &engine->scene);
}

void TrEngineTick(TrEngine *engine) {
	/**
	 * Takes care of one engine tick
	 * 
	 * @param engine The game engine context
	 */
	
	TrGraphicsUpdate(&engine->graphics, &engine->scene);
}

void TrEngineFree(TrEngine *engine) {
	/**
	 * Free the engine
	 * 
	 * @param engine The game engine context
	 */
	
	TrGraphicsFree(&engine->graphics, &engine->scene);
}

int32_t TrEngineIsRunning(TrEngine *engine) {
	/**
	 * Return if the engine should keep running or exit
	 * 
	 * @param engine The game engine context
	 * @return 1 if the engine is still running, zero if not
	 */
	
	return engine->scene.running;
}

int main(const int argc, const char *argv[]) {
	/**
	 * The main function.
	 * 
	 * @param argc Number of arguments passed to the program
	 * @param argv The arguments (as array of strings)
	 * @return Exit code
	 */
	
	TrEngine engine;
	
	TrEngineInit(&engine);
	
	// TEMP run for 10 000 ticks
	for (size_t i = 0; i < 10000; i++) {
		TrEngineTick(&engine);
		
		if (!TrEngineIsRunning(&engine)) {
			break;
		}
	}
	
	TrEngineFree(&engine);
	
	return 0;
}
