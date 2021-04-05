/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Basic Graphics Functions, to be called funcs in src/game.c
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../generic/world.h"
#include "../graphics/opengl.h"
#include "../util/alloc.h"
#include "../io/load.h"

#include "graphics.h"

GraphicsInitInfo graphics_init(void) {
	/*
	 * Initialise graphics subsystem
	 */
	GraphicsInitInfo info;
	
	info.info = (void *) gl_graphics_init();
	info.type = DG_GRAPHICS_TYPE_OPENGL;
	
	if (!info.info) {
		printf("Error: Pointer to graphics info is null.\n");
		exit(1);
	}
	
	return info;
}

void graphics_update(World *world, GraphicsInitInfo info) {
	/*
	 * Call the used graphics update function
	 */
	gl_graphics_update(world, (DgOpenGLContext *) info.info);
}

bool get_should_keep_open(GraphicsInitInfo info) {
	/*
	 * Call the used function to get if the window still needs to be open.
	 */
	return gl_get_should_keep_open((DgOpenGLContext *) info.info);
}

void graphics_free(GraphicsInitInfo info) {
	/*
	 * Free the graphics subsystem
	 */
	gl_graphics_free((DgOpenGLContext *) info.info);
}
