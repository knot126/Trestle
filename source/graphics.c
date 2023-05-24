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
#include "util/maths.h"

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
	DgVec3 position = object->shape.position;
	DgVec3 size = object->shape.size;
	DgVec4 *colour = &object->shape.colour;
	
	DgVec3 q1, q2, q3, q4, q5, q6, q7, q8;
	DgVec2 p1, p2, p3, p4, p5, p6, p7, p8;
	
	// Find the coords of the square in 3D
	q1 = (DgVec3) {position.x + size.x, position.y + size.y, position.z + size.z};
	q2 = (DgVec3) {position.x - size.x, position.y + size.y, position.z + size.z};
	q3 = (DgVec3) {position.x + size.x, position.y + size.y, position.z - size.z};
	q4 = (DgVec3) {position.x - size.x, position.y + size.y, position.z - size.z};
	q5 = (DgVec3) {position.x + size.x, position.y - size.y, position.z + size.z};
	q6 = (DgVec3) {position.x - size.x, position.y - size.y, position.z + size.z};
	q7 = (DgVec3) {position.x + size.x, position.y - size.y, position.z - size.z};
	q8 = (DgVec3) {position.x - size.x, position.y - size.y, position.z - size.z};
	
	// Convert to screen space (or whatever the fuck it is)
	p1 = (DgVec2) {q1.x / q1.z, q1.y / q1.z};
	p2 = (DgVec2) {q2.x / q2.z, q2.y / q2.z};
	p3 = (DgVec2) {q3.x / q3.z, q3.y / q3.z};
	p4 = (DgVec2) {q4.x / q4.z, q4.y / q4.z};
	p5 = (DgVec2) {q5.x / q5.z, q5.y / q5.z};
	p6 = (DgVec2) {q6.x / q6.z, q6.y / q6.z};
	p7 = (DgVec2) {q7.x / q7.z, q7.y / q7.z};
	p8 = (DgVec2) {q8.x / q8.z, q8.y / q8.z};
	
	// Align the things properly since DgBitmap doesn't use negative numbers
	// for screen space
#define PROPERALIGN(POINT) POINT = (DgVec2) {0.5f + 0.5f * POINT.x, 0.5f + 0.5f * POINT.y};
	PROPERALIGN(p1);
	PROPERALIGN(p2);
	PROPERALIGN(p3);
	PROPERALIGN(p4);
	PROPERALIGN(p5);
	PROPERALIGN(p6);
	PROPERALIGN(p7);
	PROPERALIGN(p8);
#undef PROPERALIGN
	
	// Draw the box!
	DgBitmapDrawPoint2(&this->bitmap, p1, 0.01f, &(DgColour) {1.0f, 0.0f, 0.0f, 1.0f}); // r
	DgBitmapDrawPoint2(&this->bitmap, p2, 0.01f, &(DgColour) {0.0f, 1.0f, 0.0f, 1.0f}); // g
	DgBitmapDrawPoint2(&this->bitmap, p3, 0.01f, &(DgColour) {0.0f, 0.0f, 1.0f, 1.0f}); // b
	DgBitmapDrawPoint2(&this->bitmap, p4, 0.01f, &(DgColour) {1.0f, 1.0f, 0.0f, 1.0f}); // t
	DgBitmapDrawPoint2(&this->bitmap, p5, 0.01f, &(DgColour) {0.0f, 1.0f, 1.0f, 1.0f}); // y
	DgBitmapDrawPoint2(&this->bitmap, p6, 0.01f, &(DgColour) {1.0f, 0.0f, 1.0f, 1.0f}); // p
	DgBitmapDrawPoint2(&this->bitmap, p7, 0.01f, &(DgColour) {1.0f, 1.0f, 1.0f, 1.0f}); // w
	DgBitmapDrawPoint2(&this->bitmap, p8, 0.01f, &(DgColour) {0.5f, 0.5f, 1.0f, 1.0f}); // o
	
	// Top part
	DgBitmapDrawLine(&this->bitmap, p1, p3, colour);
	DgBitmapDrawLine(&this->bitmap, p1, p2, colour);
	DgBitmapDrawLine(&this->bitmap, p2, p4, colour);
	DgBitmapDrawLine(&this->bitmap, p4, p1, colour);
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
