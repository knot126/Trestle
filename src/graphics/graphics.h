/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics Subsystem
 */

#pragma once

#include <stdbool.h>

#include "util/maths.h"
#include "graph/graph.h"
#include "graphics/texture.h"
#include "graphics/gl_incl.h"
#include "graphics/vertex1.h"
#include "types.h"

typedef struct Mesh {
	float * vert;
	uint32_t * index;
	const char * texture;
	
	uint32_t vert_count;
	uint32_t index_count;
	uint32_t vbo, ebo, vao;
	bool updated;
	bool shouldFree;
} Mesh;

typedef struct TextString {
	float *vertex;
	size_t vertex_count;
	float *index;
	size_t index_count;
	
	float size;
	const char *text;
	size_t length;
} TextString;

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
	Name camera;
	
	// Mesh names
	Name  *mesh_name;
	Mesh  *mesh;
	size_t mesh_count;
	
	// Curve objects
	Curve *curve;
	size_t curve_count;
} GraphicsSystem;

typedef GraphicsSystem DgOpenGLContext;

GraphicsSystem *graphics(GraphicsSystem *G);

void graphics_init(GraphicsSystem * restrict gl);
void graphics_update(GraphicsSystem * restrict gl, SceneGraph * restrict graph);
void graphics_free(GraphicsSystem *gl);
void gl_handle_input(GraphicsSystem *gl);
bool get_should_keep_open(GraphicsSystem *gl);

void graphics_set_background(GraphicsSystem * restrict gl, const DgVec4 colour);
void graphics_set_camera(GraphicsSystem * restrict gl, const uint32_t id);
uint32_t graphics_get_camera(GraphicsSystem * restrict gl);

void graphics_add_curve(GraphicsSystem * restrict gl, DgVec3 p0, DgVec3 p1, DgVec3 p2, DgVec3 p3);
Name graphics_create_mesh(GraphicsSystem * restrict gl, Name name);
Name graphics_set_mesh(GraphicsSystem * restrict gl, Name name, size_t vertex_count, QRVertex1 *vertex, size_t index_count, uint32_t *index, const char *texture);
Mesh * const graphics_get_mesh(GraphicsSystem * restrict gl, Name name);
