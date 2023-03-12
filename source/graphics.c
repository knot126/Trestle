/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Graphics handling
 * 
 * @todo Everything should return DgError
 * @todo Once TrScene is ready, this should use the actual scene type
 */

#include "includes.h"

#include "util/log.h"
#include "util/bitmap.h"
#include "util/window.h"
#include "util/rand.h" // TEMP

#include "scene.h"

#include "graphics.h"

void TrGraphicsInit(TrGraphics *this, TrScene *scene) {
	/**
	 * Initialise the graphics and windowing subsystem
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
	
	DgBitmapSetFlags(&this->bitmap, DG_BITMAP_DRAWING_ALPHA);
	
	DgWindowAssocaiteBitmap(&this->window, &this->bitmap);
	
	DgBitmapFill(&this->bitmap, (DgColour) {0.0f, 0.0f, 0.0f, 1.0f});
}

void TrGraphicsUpdate(TrGraphics *this, TrScene *scene) {
	/**
	 * Update the window with any new graphics
	 * 
	 * @param this Graphics subsystem object
	 * @param scene Scene to apply to (holds all context-specific state)
	 */
	
	DgBitmapDrawLine(&this->bitmap, (DgVec2) {DgRandFloat(), DgRandFloat()}, (DgVec2) {DgRandFloat(), DgRandFloat()}, &(DgColour) {DgRandFloat(), DgRandFloat(), DgRandFloat(), 1.0f});
	
	uint32_t status = DgWindowUpdate(&this->window, NULL);
	
	if (status < 0) {
		DgLog(DG_LOG_ERROR, "Failed to update window.");
		return;
	}
	
	bool running = (status == 0);
	
	// Update if the game engine is running
	if (!running) {
		DgValue key; DgValueStaticString(&key, "running");
		DgValue value; DgValueBool(&value, running);
		
		DgError status = DgTableSet(&scene->info, &key, &value);
		
		if (status) {
			DgLog(DG_LOG_WARNING, "TrGraphicsUpdate [%d]: %d", __LINE__, status);
		}
	}
}

void TrGraphicsFree(TrGraphics *this, TrScene *scene) {
	/**
	 * Release all reosurces assocaited with the graphics system
	 * 
	 * @param this Graphics system object
	 * @param scene Scene to apply to (holds all context-specific data)
	 */
	
	DgWindowFree(&this->window);
}
