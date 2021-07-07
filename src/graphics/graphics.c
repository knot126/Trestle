/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics System Wrapper - this is still here for legacy reasons
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "world/world.h"
#include "graphics/opengl.h"
#include "util/alloc.h"

#include "graphics.h"

GraphicsSystem *graphics_init(void) {
	/*
	 * Initialise graphics subsystem
	 */
	
	return gl_graphics_init();
}

void graphics_update(GraphicsSystem *this, World *world) {
	/*
	 * Call the used graphics update function
	 */
	gl_graphics_update(this, world);
}

bool get_should_keep_open(GraphicsSystem *this) {
	/*
	 * Call the used function to get if the window still needs to be open.
	 */
	return gl_get_should_keep_open(this);
}

void graphics_free(GraphicsSystem *this) {
	/*
	 * Free the graphics subsystem
	 */
	gl_graphics_free(this);
}

