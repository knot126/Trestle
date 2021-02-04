/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file is part of the Decent Games Engine. Please do not redistribute the
 * sources without prior permission from Decent Games and/or contributours.
 * 
 * Contributours:
 *  - cdde <cddepppp256@protonmail.com>
 * 
 * Main File
 * =========
 * 
 * This file holds the main function and therefore the call to platform_main.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "graphics/vulkan.h"
#include "graphics/opengl.h"
#include "graphics/graphics.h"
#include "util/alloc.h"

const bool graphics_gl = true;
bool should_keep_open = true;

static void print_info(void) {
	printf("Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

static int game_loop(void) {
	/* The main loop. */
	should_keep_open = false;
}

int game_main(int argc, char* argv[]) {
	/* The first real main game function, called from the main() function of the
	 * OS. */
	DgVulkanInfo* vk;
	DgOpenGLContext* gl;
	
	print_info();
	
	// Create a basic memory pool
	// NOTE that this should be refactored for preformance!
	printf("Making initial memory pool (1 MiB)...\n");
	uint16_t mp = DgMakePool(1024 * 1024);
	
	// Main loop
	while (should_keep_open) {
		game_loop();
	}
	
	// Graphics initialisation
	printf("Init graphics subsystem...\n");
	if (!graphics_gl) {
		DgVulkanInfo* vk = graphics_init();
		
		if (!vk) {
			printf("Error: Pointer to vulkan info is noll.\n");
			exit(1);
		}
	}
	else if (graphics_gl) {
		DgOpenGLContext* gl = gl_graphics_init();
		
		if (!gl) {
			printf("Error: Pointer to OpenGL info is noll.\n");
			exit(1);
		}
	}
	
	// Graphics destruction
	printf("Destroying graphics subsystem...");
	if (!graphics_gl) {
		graphics_free(vk);
	}
	else if (graphics_gl) {
		gl_graphics_free(gl);
	}
	printf("  Done\n");
	
	// Free pool
	printf("Free memory pool...\n");
	DgFreePool(mp);
	
	return 0;
}
