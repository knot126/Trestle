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

#include <GLFW/glfw3.h>

#include "graphics/vulkan.h"
#include "graphics/opengl.h"
#include "graphics/graphics.h"
#include "util/alloc.h"
#include "util/bag.h"

const bool graphics_gl = true;
bool should_keep_open = true;

static void print_info(void) {
	printf("Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

static int game_loop(void* pGInfo) {
	/* The main loop. */
	DgVulkanInfo* vk = NULL;
	DgOpenGLContext* gl = NULL;
	bool should_keep_open = true;
	
	if (!pGInfo) {
		printf("Graphics pointer is null.\n");
	}
	
	// Set the proper value based on being GraphicsGL or not
	if (!graphics_gl) {
		vk = (DgVulkanInfo *) pGInfo;
	}
	else if (graphics_gl) {
		gl = (DgOpenGLContext *) pGInfo;
	}
	
	while (should_keep_open) {
		
		// TODO: Make a function that does all of the graphics stuff
		if (!graphics_gl) {
			should_keep_open = false;
		}
		else if (graphics_gl) {
			should_keep_open = !glfwWindowShouldClose(gl->window);
			glfwSwapBuffers(gl->window);
			glfwPollEvents();
			
			if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(gl->window, GL_TRUE);
			}
		}
		
	} // while (should_keep_open)
	
	return 0;
}

int game_main(int argc, char* argv[]) {
	/* The first real main game function, called from the main() function of the
	 * OS. */
	DgVulkanInfo* vk;
	DgOpenGLContext* gl;
	
	print_info();
	
	// Create a basic memory pool
	// NOTE: This should be refactored for preformance!
	printf("Making initial memory pool (1 MiB)...\n");
	alloch_t mp = DgMakePool(1024 * 1024);
	
	// Graphics initialisation
	printf("Init graphics subsystem...\n");
	if (!graphics_gl) {
		vk = graphics_init();
		
		if (!vk) {
			printf("Error: Pointer to vulkan info is noll.\n");
			exit(1);
		}
	}
	else if (graphics_gl) {
		gl = gl_graphics_init();
		
		if (!gl) {
			printf("Error: Pointer to OpenGL info is noll.\n");
			exit(1);
		}
	}
	
	// Main loop
	printf("Main loop\n");
	if (!graphics_gl) {
		game_loop(vk);
	}
	else if (graphics_gl) {
		game_loop(gl);
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
