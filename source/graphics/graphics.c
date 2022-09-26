/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Graphics handling
 */

#include "util/log.h"
#include "util/bitmap.h"
#include "util/window.h"
#include "util/rand.h" // TEMP

#include "graphics.h"

void TrGraphicsInit(TrGraphics *this, void *scene) {
	/**
	 * Initialise the graphics and windowing subsystem
	 * 
	 * @todo This should return DgError
	 * @todo Once TrScene is ready, this should use the actual scene type
	 * 
	 * @param this Graphics subsystem object
	 * @param scene Scene to apply to (holds all context-specific state)
	 */
	
	if (DgWindowInit(&this->window, "Trestle Engine", (DgVec2I) {1280, 720})) {
		DgLog(DG_LOG_ERROR, "Failed to create window.");
		return;
	}
	
	// Temp bitmap for testing
	if (DgBitmapInit(&this->bitmap, 1280, 720, 3)) {
		DgLog(DG_LOG_ERROR, "Failed to create bitmap for window.");
		return;
	}
	
	DgBitmapFill(&this->bitmap, (DgColour) {0.0f, 0.0f, 0.0f, 1.0f});
}

void TrGraphicsUpdate(TrGraphics *this, void *scene) {
	/**
	 * Update the window with any new graphics
	 * 
	 * @todo This should return DgError
	 * @todo Once TrScene is ready, this should use the actual scene type
	 * 
	 * @param this Graphics subsystem object
	 * @param scene Scene to apply to (holds all context-specific state)
	 */
	
	if (DgWindowUpdate(&this->window, &this->bitmap) < 0) {
		DgLog(DG_LOG_ERROR, "Failed to update window.");
		return;
	}
	
	// TEMP for hello world
	DgBitmapDrawLine(&this->bitmap, (DgVec2) {DgRandFloat(), DgRandFloat()}, (DgVec2) {DgRandFloat(), DgRandFloat()}, &(DgColour) {DgRandFloat(), DgRandFloat(), DgRandFloat(), 1.0f});
}

void TrGraphicsFree(TrGraphics *this, void *scene) {
	/**
	 * Release all reosurces assocaited with the graphics system
	 * 
	 * @todo Everything from above should also apply here
	 * 
	 * @param this Graphics system object
	 * @param scene Scene to apply to (holds all context-specific data)
	 */
	
	DgWindowFree(&this->window);
}
