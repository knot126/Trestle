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

typedef struct Curve {
	DgVec3 points[4];
} Curve;

typedef struct GraphicsSystem {
	/**
	 * An OpenGL-based graphics system wrapper, which takes care of most of the
	 * graphics stuff.
	 */
	
	// Window and subsystems
	GLFWwindow* window;
	OpenGLTextureManager texture;
	
	// Programs
	unsigned* programs;
	size_t programs_count;
	
	// Shaders (not needed?)
	unsigned* shaders;
	size_t shader_count;
	
	// System's global state
	DgVec4 clearColour;
	uint32_t cam_active[3];
	
	// Curve objects
	Curve *curve;
	size_t curve_count;
	
	// DEPRECATED
	World *world;
} GraphicsSystem;

typedef GraphicsSystem DgOpenGLContext;

GraphicsSystem *graphics(GraphicsSystem *G);

void graphics_init(GraphicsSystem * restrict gl, const World * restrict world);
void graphics_update(GraphicsSystem *gl);
void graphics_free(GraphicsSystem *gl);
void gl_handle_input(GraphicsSystem *gl);
bool get_should_keep_open(GraphicsSystem *gl);

void graphics_set_background(GraphicsSystem * restrict gl, const DgVec4 colour);
void graphics_set_camera(GraphicsSystem * restrict gl, const uint32_t id);
uint32_t graphics_get_camera(GraphicsSystem * restrict gl);

void graphics_add_curve(GraphicsSystem * restrict gl, DgVec3 p0, DgVec3 p1, DgVec3 p2, DgVec3 p3);
