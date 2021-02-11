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

#include "../graphics/vulkan.h"
#include "../graphics/opengl.h"
#include "../util/alloc.h"
#include "../io/load.h"

#include "graphics.h"

GraphicsInitInfo graphics_init(GraphicsType type) {
	GraphicsInitInfo info;
	
	if (type == DG_GRAPHICS_TYPE_VULKAN) {
		info.info = (void *) vk_graphics_init();
		info.type = DG_GRAPHICS_TYPE_VULKAN;
	}
	
	if (type == DG_GRAPHICS_TYPE_OPENGL) {
		info.info = (void *) gl_graphics_init();
		info.type = DG_GRAPHICS_TYPE_OPENGL;
	}
	
	if (!info.info) {
		printf("Error: Pointer to graphics info is null.\n");
		exit(1);
	}
	
	return info;
}

void graphics_update(GraphicsInitInfo info) {
	if (info.type == DG_GRAPHICS_TYPE_OPENGL) {
		gl_graphics_update((DgOpenGLContext *) info.info);
	}
}

bool get_should_keep_open(GraphicsInitInfo info) {
	if (info.type == DG_GRAPHICS_TYPE_OPENGL) {
		return gl_get_should_keep_open((DgOpenGLContext *) info.info);
	}
	
	return false;
}

void graphics_free(GraphicsInitInfo info) {
	if (info.type == DG_GRAPHICS_TYPE_VULKAN) {
		vk_graphics_free((DgVulkanInfo *) info.info);
	}
	
	if (info.type == DG_GRAPHICS_TYPE_OPENGL) {
		gl_graphics_free((DgOpenGLContext *) info.info);
	}
}
