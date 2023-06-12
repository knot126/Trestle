/**
 * Copyright (C) 2022 - 2023 Knot126
 * =================================
 * 
 * Main functions
 */

#include "includes.h"

#include "util/log.h"
#include "util/time.h"

#include "scene.h"
#include "graphics.h"

#include "main.h"

#define __TRESTLE_STRING_BOYKISSER_TOTEM "BOYKISSERTOTEM"

void TrEngineInit(TrEngine *engine) {
	/**
	 * Starts the engine
	 * 
	 * @param engine The game engine context
	 */
	
	DgError status;
	
	DgLog(DG_LOG_VERBOSE, "TrSceneInit");
	status = TrSceneInit(&engine->scene);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to initialise scene (status = %d)", status);
	}
	
	char name[] = "object0";
	
	for (size_t i = 0; i < 9; i++) {
		TrObject *object;
		
		if ((status = TrSceneCreateObject(&engine->scene, name, &object))) {
			DgLog(DG_LOG_ERROR, "Errur, Fail to create objet!!! %d", status);
		}
		
		name[6]++;
		
		if ((status = TrObjectSetShape(object, & (TrAABBShape) {
			.position = (DgVec3) {0.0f + (1.1f * i), 0.0f + (1.1f * i), 3.0f + (1.1f * i)},
			.size = (DgVec3) {1.0f + (0.05f * i), 1.0f + (0.05f * i), 1.0f + (0.05f * i)},
			.colour = (DgVec4) {1.0f - (0.08f * i), 1.0f, 1.0f - (0.08f * i), 1.0f}
		}))) {
			DgLog(DG_LOG_ERROR, "Fail to set shape!!! %s", DgErrorString(status));
		}
	}
	
	DgLog(DG_LOG_VERBOSE, "TrGraphicsInit");
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
	
	DgValue key; DgValueStaticString(&key, "running");
	DgValue value;
	
	DgTableGet(&engine->scene.info, &key, &value);
	
	return value.data.asBool;
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
	
	// Print out engine version
	DgLog(DG_LOG_INFO, "Trestle Engine %d.%d.%d", TR_VERSION_MAJOR, TR_VERSION_MINOR, TR_VERSION_PATCH);
	
	// Initialise melon library
	TrInitGlobals();
	
	// Main engine init
	TrEngine engine;
	
	DgLog(DG_LOG_VERBOSE, "TrEngineInit");
	TrEngineInit(&engine);
	
	// Main loop
	while (TrEngineIsRunning(&engine)) {
		double start = DgTime();
		
		TrEngineTick(&engine);
		
		double delta = DgTime() - start;
		
#if defined(TR_ENABLE_FRAME_TIME_LOG)
		DgLog(DG_LOG_INFO, ":: Frame time %.4fms (%.2f fps) ::", delta * 1000.0f, 1.0f / delta);
#endif
	}
	
	// Free the engine
	DgLog(DG_LOG_VERBOSE, "TrEngineFree");
	TrEngineFree(&engine);
	
	return 0;
}
