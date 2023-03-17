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
}

static void TrGraphicsUpdate_DrawObject(TrGraphics *this, TrScene *scene, TrObject *object) {
	DgVec2 position = object->shape.position;
	DgVec2 size = object->shape.size;
	DgVec4 *colour = &object->shape.colour;
	
	DgVec2 p1, p2, p3, p4;
	
	// Find the coords of the square
	p1.x = position.x - size.x;
	p1.y = position.y + size.y;
	p2.x = position.x - size.x;
	p2.y = position.y - size.y;
	p3.x = position.x + size.x;
	p3.y = position.y - size.y;
	p4.x = position.x + size.x;
	p4.y = position.y + size.y;
	
	// Draw the box!
	DgBitmapDrawLine(&this->bitmap, p1, p2, colour);
	DgBitmapDrawLine(&this->bitmap, p2, p3, colour);
	DgBitmapDrawLine(&this->bitmap, p3, p4, colour);
	DgBitmapDrawLine(&this->bitmap, p4, p1, colour);
	
	DgBitmapDrawLine(&this->bitmap, p1, p3, colour);
	DgBitmapDrawLine(&this->bitmap, p2, p4, colour);
}

static void TrGraphicsUpdate_DrawWorld(TrGraphics *this, TrScene *scene) {
	DgError status;
	
	DgBitmapFill(&this->bitmap, (DgColour) {0.0f, 0.0f, 0.0f, 1.0f});
	
	size_t index = 0;
	
	while (true) {
		TrObject *object;
		
		if ((status = TrSceneAtObject(scene, index, &object))) {
			break;
		}
		
		TrGraphicsUpdate_DrawObject(this, scene, object);
		
		index++;
	}
	
	//DgLog(DG_LOG_INFO, "Done drawing %d objects. (status = %s)", index, DgErrorString(status));
}

static void TrGraphicsUpdate_WindowStatus(TrScene *scene, int32_t status) {
	bool running = (status == 0);
	
	if (!running) {
		DgValue key; DgValueStaticString(&key, "running");
		DgValue value; DgValueBool(&value, running);
		
		DgError status = DgTableSet(&scene->info, &key, &value);
		
		if (status) {
			DgLog(DG_LOG_WARNING, "TrGraphicsUpdate [%d]: %d", __LINE__, status);
		}
	}
}

void TrGraphicsUpdate(TrGraphics *this, TrScene *scene) {
	/**
	 * Update the window with any new graphics
	 * 
	 * @param this Graphics subsystem object
	 * @param scene Scene to apply to (holds all context-specific state)
	 */
	
	TrGraphicsUpdate_DrawWorld(this, scene);
	
	int32_t status = DgWindowUpdate(&this->window, NULL);
	
	if (status < 0) {
		DgLog(DG_LOG_ERROR, "Failed to update window.");
		return;
	}
	
	// Update if the game engine is running
	TrGraphicsUpdate_WindowStatus(scene, status);
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
