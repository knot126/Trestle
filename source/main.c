/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Main functions
 */

#include "util/log.h"

#include "graphics/graphics.h"

#include "main.h"

void TrEngineInit(TrEngine *engine) {
	/**
	 * Starts the engine
	 * 
	 * @param engine The game engine context
	 */
	
	TrGraphicsInit(&engine->graphics, NULL);
}

void TrEngineTick(TrEngine *engine) {
	/**
	 * Takes care of one engine tick
	 * 
	 * @param engine The game engine context
	 */
	
	TrGraphicsUpdate(&engine->graphics, NULL);
}

void TrEngineFree(TrEngine *engine) {
	/**
	 * Free the engine
	 * 
	 * @param engine The game engine context
	 */
	
	TrGraphicsFree(&engine->graphics, NULL);
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
	}
	
	TrEngineFree(&engine);
	
	return 0;
}
