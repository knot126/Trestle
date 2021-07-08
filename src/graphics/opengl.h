/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#pragma once

#include <stdbool.h>

#include "util/maths.h"
#include "world/world.h"
#include "graphics/texture.h"
#include "graphics/gl_incl.h"

typedef struct DgOpenGLContext {
	/**
	 * An OpenGL-based graphics system wrapper, which takes care of most of the
	 * graphics stuff.
	 */
	
	GLFWwindow* window;
	OpenGLTextureManager texture;
	
	unsigned* programs;
	size_t programs_count;
	
	unsigned* shaders;
	size_t shader_count;
	
	DgVec4 clearColour;
} DgOpenGLContext;

typedef DgOpenGLContext GraphicsSystem;

GraphicsSystem *gl_graphics_init(void);
void gl_graphics_update(GraphicsSystem *gl, World *world);
void gl_graphics_free(GraphicsSystem *gl);
void gl_handle_input(GraphicsSystem *gl);
bool gl_get_should_keep_open(GraphicsSystem *gl);
