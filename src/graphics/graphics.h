/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics Subsystem
 */

#pragma once

#include <stdbool.h>

#include "util/maths.h"
#include "util/surface.h"
#include "graph/graph.h"
#include "graphics/texture.h"
#include "graphics/gl_incl.h"
#include "graphics/vertex3d.h"
#include "graphics/vertex2d.h"
#include "types.h"

/**
 * Graphics system flags
 */
enum {
	QR_GRAPHICS_DRAW_MESHES_AS_LINES = (1 << 0),
	QR_GRAPHICS_DRAW_CURVES_AS_VERTEXISATIONS = (1 << 1),
};

/**
 * A 3D mesh that is drawn in the 3D world.
 * 
 * For compatibility reasons, vert is a float* and vertex is QRVertex3D*. Always
 * use vertex (QRVertex3D) now.
 */
typedef struct Mesh3D {
	union {
		float *vert;
		QRVertex3D *vertex;
	};
	uint32_t *index;
	const char *texture;
	
	union {
		uint32_t vert_count, vertex_count;
	};
	uint32_t index_count;
	uint32_t vbo, ebo, vao;
	bool updated;
} Mesh3D;

// Compatiblity
typedef Mesh3D Mesh;

/**
 * A 3D first-class surface represented by a single or group of Bézier surfaces.
 */
typedef struct Surface3D {
	DgSurface3D *surface;
	uint32_t     surface_count;
	uint32_t     surface_alloc;
	
	struct {
		QRVertex3D *vertex;
		uint32_t *index;
		
		uint32_t vertex_count;
		uint32_t index_count;
		
		uint32_t vbo, ebo, vao;
		bool updated;
	} cache;
} Surface3D;

/**
 * A 2D mesh that is displayed as part of the GUI layer.
 */
typedef struct Mesh2D {
	QRVertex2D *vertex;
	uint32_t *index;
	const char *texture;
	
	uint32_t vertex_count;
	uint32_t index_count;
	uint32_t vbo, ebo, vao;
	bool updated;
} Mesh2D;

/**
 * DEPRECATED: The current curves system was only for testing as a proof of
 * concept. It's dead.
 */
typedef struct Curve {
	DgVec3 points[4];
} Curve;

typedef struct GraphicsSystem {
	/**
	 * An OpenGL-based graphics system wrapper, which takes care of most of the
	 * graphics stuff.
	 */
	
	// Window and subsystems
	GLFWwindow *window;
	OpenGLTextureManager texture;
	
	// Programs
	unsigned *programs;
	size_t programs_count;
	
	// Shaders (not needed?)
	unsigned *shaders;
	size_t shader_count;
	
	// System's global state
	DgVec4 clearColour;
	Name camera;
	float curve_render_quality;
	float camera_fov;
	size_t frame;
	uint64_t flags;
	
	// Mesh3D objects
	Name  *mesh_name;
	Mesh  *mesh;
	size_t mesh_count;
	size_t mesh_alloc;
	
	// Surface3D objects
	Name      *surface_name;
	Surface3D *surface;
	size_t     surface_count;
	size_t     surface_alloc;
	
	// Mesh2D objects
	Name    *mesh2d_name;
	Mesh2D  *mesh2d;
	size_t   mesh2d_alloc;
	size_t   mesh2d_count;
	
	// Curve objects
	Curve *curve;
	size_t curve_count;
} GraphicsSystem;

typedef GraphicsSystem DgOpenGLContext;

// Graphics System API
void graphics_init(GraphicsSystem * restrict gl);
void graphics_update(GraphicsSystem * restrict gl, SceneGraph * restrict graph);
void graphics_free(GraphicsSystem *gl);
void gl_handle_input(GraphicsSystem *gl);
bool get_should_keep_open(GraphicsSystem *gl);

// Global Graphics State
void graphics_set_background(GraphicsSystem * restrict gl, const DgVec4 colour);
void graphics_set_camera(GraphicsSystem * restrict gl, const uint32_t id);
uint32_t graphics_get_camera(GraphicsSystem * restrict gl);
DgVec3 graphics_get_camera_forward(GraphicsSystem * restrict gl, SceneGraph * restrict graph, const DgVec3 *forward);
DgVec2I graphics_get_screen_size(GraphicsSystem * restrict gl);
void graphics_set_mouse_disabled(GraphicsSystem * restrict gl, bool enabled);
void graphics_set_curve_render_quality(GraphicsSystem * restrict graphics, float quality);
void graphics_set_fov(GraphicsSystem * restrict graphics, float fov);

// Curves
void graphics_add_curve(GraphicsSystem * restrict gl, DgVec3 p0, DgVec3 p1, DgVec3 p2, DgVec3 p3);

// 3D Meshes
Name graphics_create_mesh(GraphicsSystem * restrict gl, Name name);
Name graphics_destroy_mesh(GraphicsSystem * restrict gl, Name name);
Name graphics_set_mesh(GraphicsSystem * restrict gl, Name name, size_t vertex_count, QRVertex1 *vertex, size_t index_count, uint32_t *index, const char *texture);
Mesh * const graphics_get_mesh(GraphicsSystem * restrict gl, Name name);
size_t graphics_get_mesh_counts(GraphicsSystem * restrict gl, size_t *allocsz);

// 3D Surface
Name graphics_create_surface3d(GraphicsSystem * const restrict gl, const Name name);
Name graphics_add_patch_to_surface3d(GraphicsSystem * const restrict gl, const Name name, DgSurface3D * const restrict patch);
Name graphics_create_patch_surface3d(GraphicsSystem * const restrict gl, const Name name, const uint32_t x, const uint32_t y, const DgVec3 * const restrict points);
Surface3D * const graphics_get_surface3d(GraphicsSystem * const restrict gl, const Name name);
size_t graphics_get_surface3d_counts(const GraphicsSystem * const restrict gl, size_t *allocsz);

// 2D Meshes
Name graphics_create_mesh2d(GraphicsSystem * restrict gl, Name name);
Name graphics_set_mesh2d(GraphicsSystem * restrict gl, Name name, uint32_t vertex_count, QRVertex2D *vertex, uint32_t index_count, uint32_t *index, const char *texture);
Mesh2D * const graphics_get_mesh2d(GraphicsSystem * restrict gl, Name name);
size_t graphics_get_mesh2d_counts(GraphicsSystem * restrict gl, size_t *allocsz);
