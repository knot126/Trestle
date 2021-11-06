/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics System
 */

/**
 * Graphics system TODO:
 * 
 *   * Refactoring graphics system: a lot of this system was written when I had
 *     just started programming; there are things that can be done much better.
 *   * Abstract away GLFW related things: it would be nice to have a more
 *     generic interface to the windowing and input library.
 *   * Speaking of that, unbind input and graphics system if it is possible at
 *     all.
 *   * Stop making OpenGL calls directly: This is bad for various reasons, 
 *     mainly that there is a very hard dependency on a very much dying library.
 * 
 * And so much more...
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "misc/grpreq.h"
#include "graphics/vertex3d.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/str.h"
#include "util/bitmap.h"
#include "util/log.h"
#include "util/surface.h"
#include "util/rand.h"
#include "window/window.h"
#include "types.h" // For g_deltaTime

#include "graph/graph.h"

#include "graphics.h"

void graphics_init(GraphicsSystem * restrict this) {
	/**
	 * Initialise graphics system
	 */
	
}

void graphics_update(GraphicsSystem * restrict this, SceneGraph * restrict graph) {
	/*
	 * Update the entire graphics system
	 */
	
}

void graphics_handle_input(GraphicsSystem* this) {
	/*
	 * Handles most game input, for example keyboard input
	 */
	
}

void graphics_free(GraphicsSystem* this) {
	/**
	 * Free the graphics subsystem.
	 */
	
}

bool get_should_keep_open(GraphicsSystem *info) {
	/**
	 * Return true if the window should be kept open.
	 */
	
	return true;
}

void graphics_set_background(GraphicsSystem * restrict this, const DgVec4 colour) {
	/**
	 * Setter function for the graphics system's background clear colour.
	 */
	
	this->clearColour = colour;
}

void graphics_set_camera(GraphicsSystem * restrict this, Name name) {
	/*
	 * Setter function for the world's active camera.
	 * 
	 * Implementation note: We add one to I so that we can check if no camera
	 * has been set and use default matrix in that case.
	 */
	
	
}

Name graphics_get_camera(GraphicsSystem * restrict this) {
	/**
	 * Getter function for the active camera's ID.
	 */
	
	return 0;
}

DgVec3 graphics_get_camera_forward(GraphicsSystem * restrict this, SceneGraph * restrict graph, const DgVec3 *forward) {
	/**
	 * From a vector pointing to the real given direction, retrive a vector
	 * relative to the camera's rotation.
	 */
	
	return (DgVec3) {0.0f, 1.0f, 0.0f};
}

DgVec2I graphics_get_screen_size(GraphicsSystem * restrict this) {
	/**
	 * The the size of the window.
	 */
	
	return (DgVec2I) {0, 0};
}

void graphics_set_mouse_disabled(GraphicsSystem * restrict this, bool enabled) {
	/**
	 * Disable the GLFW mouse controls.
	 */
	
}

void graphics_set_curve_render_quality(GraphicsSystem * restrict graphics, float quality) {
	/**
	 * Set the render quality of curves in samples per cube unit.
	 */
	
	
}

void graphics_set_fov(GraphicsSystem * restrict graphics, float fov) {
	/**
	 * Set the camera FoV in turns. That is, 1.0 turn = 2pi rad = 360 deg.
	 */
	
	
}

/**
 * =============================================================================
 * Legacy Curves
 * =============================================================================
 */

void graphics_add_curve(GraphicsSystem * restrict this, DgVec3 p0, DgVec3 p1, DgVec3 p2, DgVec3 p3) {
	/**
	 * DEPRECATED: This API will be updated soon.
	 */
	
}

/**
 * =============================================================================
 * 3D Meshes
 * =============================================================================
 */

static int graphics_realloc_mesh(GraphicsSystem * restrict this) {
	/**
	 * Take care of reallocating mesh data when low on space. It must make sure
	 * there is at least one free mesh worth of space.
	 */
	
	if (this->mesh_count >= this->mesh_alloc) {
		this->mesh_alloc = 4 + (this->mesh_alloc * 2);
		this->mesh = DgRealloc(this->mesh, sizeof *this->mesh * this->mesh_alloc);
		
		if (!this->mesh) {
			return 1;
		}
		
		this->mesh_name = DgRealloc(this->mesh_name, sizeof *this->mesh_name * this->mesh_alloc);
		
		if (!this->mesh_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graphics_find_mesh(GraphicsSystem * restrict this, Name name) {
	/**
	 * Find a mesh that goes by a given name.
	 */
	
	for (size_t i = 0; i < this->mesh_count; i++) {
		if (this->mesh_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graphics_create_mesh(GraphicsSystem * restrict this, Name name) {
	/**
	 * Creates a mesh entity.
	 */
	
	if (graphics_realloc_mesh(this)) {
		return 0;
	}
	
	this->mesh_name[this->mesh_count++] = name;
	memset(&this->mesh[this->mesh_count - 1], 0, sizeof *this->mesh);
	
	return name;
}

Name graphics_destroy_mesh(GraphicsSystem * restrict this, Name name) {
	/**
	 * Virtually distroys a mesh, clearing its chunk of memory for later use.
	 * Returns name on successful free, zero on failure to free.
	 */
	
	size_t idx = graphics_find_mesh(this, name);
	
	if (idx == -1) {
		return 0;
	}
	
	this->mesh_name[idx] = 0;
	
	Mesh *mesh = &this->mesh[idx];
	
	if (mesh->vertex) {
		DgFree(mesh->vertex);
	}
	
	if (mesh->index) {
		DgFree(mesh->index);
	}
	
	if (mesh->texture) {
		DgFree((void *) mesh->texture);
	}
	
// 	thisDeleteBuffers(1, &mesh->vbo);
// 	thisDeleteBuffers(1, &mesh->ebo);
// 	thisDeleteVertexArrays(1, &mesh->vao);
	
	return name;
}

Name graphics_set_mesh(GraphicsSystem * restrict this, Name name, size_t vertex_count, QRVertex1 *vertex, size_t index_count, uint32_t *index, const char *texture) {
	/**
	 * Set mesh data for a given mesh.
	 */
	
	size_t loc = graphics_find_mesh(this, name);
	
	if (loc == -1) {
		return 0;
	}
	
	this->mesh[loc].vert = (float *) vertex;
	this->mesh[loc].vert_count = vertex_count;
	this->mesh[loc].index = index;
	this->mesh[loc].index_count = index_count;
	this->mesh[loc].texture = DgStrdup(texture);
	this->mesh[loc].updated = true;
	
	return name;
}

Mesh * const graphics_get_mesh(GraphicsSystem * restrict this, Name name) {
	/**
	 * Get a reference to a mesh structure for a given name.
	 */
	
	size_t index = graphics_find_mesh(this, name);
	
	if (index == -1) {
		return NULL;
	}
	
	return &this->mesh[index];
}

size_t graphics_get_mesh_counts(GraphicsSystem * restrict this, size_t *allocsz) {
	/**
	 * Get the count of meshes that are active and optionally the count of meshes
	 * that are allocated.
	 */
	
	if (allocsz) {
		*allocsz = this->mesh_alloc;
	}
	
	return this->mesh_count;
}

/**
 * =============================================================================
 * 3D Surfaces
 * =============================================================================
 */

static int graphics_realloc_surface3d(GraphicsSystem * const restrict this) {
	/**
	 * Reallocate the list of surface entities, making sure there is room for
	 * at least one more surface.
	 */
	
	if (this->surface_count >= this->surface_alloc) {
		this->surface_alloc = 4 + (this->surface_alloc * 2);
		this->surface = DgRealloc(this->surface, sizeof *this->surface * this->surface_alloc);
		
		if (!this->surface) {
			return 1;
		}
		
		this->surface_name = DgRealloc(this->surface_name, sizeof *this->surface_name * this->surface_alloc);
		
		if (!this->surface_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graphics_find_surface3d(const GraphicsSystem * const restrict this, const Name name) {
	/**
	 * Find a surface index given its name.
	 */
	
	for (size_t i = 0; i < this->surface_count; i++) {
		if (this->surface_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graphics_create_surface3d(GraphicsSystem * const restrict this, const Name name) {
	/**
	 * Create a 3D surface.
	 */
	
	if (graphics_realloc_surface3d(this)) {
		return 0;
	}
	
	this->surface_name[this->surface_count] = name;
	memset(&this->surface[this->surface_count], 0, sizeof *this->surface);
	
	this->surface[this->surface_count].cache.updated = true;
	
	this->surface_count++;
	
	return name;
}

Name graphics_add_patch_to_surface3d(GraphicsSystem * const restrict this, const Name name, DgSurface3D * const restrict patch) {
	/**
	 * Add a sinthise patch to a Surface3D object. This takes the given object, so
	 * internal memory for it must not be freed.
	 */
	
	size_t index = graphics_find_surface3d(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	Surface3D *surface = &this->surface[index];
	
	// Reallocate patches
	if (surface->surface_alloc <= surface->surface_count) {
		surface->surface_alloc = 2 + (surface->surface_alloc * 2) - (surface->surface_alloc / 2);
		surface->surface = DgRealloc(surface->surface, sizeof *surface->surface * surface->surface_alloc);
		
		if (!surface->surface) {
			return 0;
		}
	}
	
	// Set new patch
	surface->surface[surface->surface_count] = *patch;
	
	// Increment number of patches
	surface->surface_count++;
	
	// Set as updated
	surface->cache.updated = true;
	
	return name;
}

Name graphics_create_patch_surface3d(GraphicsSystem * const restrict this, const Name name, const uint32_t x, const uint32_t y, const DgVec3 * const restrict points) {
	/**
	 * Create a surface from the x-count, y-count and points.
	 */
	
	DgSurface3D s;
	
	if (!DgSurface3DInit(&s, x, y)) {
		return 0;
	}
	
	for (uint32_t i = 0; i < x; i++) {
		for (uint32_t j = 0; j < y; j++) {
			DgSurface3DSetPoint(&s, i, j, &points[(x * i) + j]);
		}
	}
	
	if (!graphics_add_patch_to_surface3d(this, name, &s)) {
		DgLog(DG_LOG_ERROR, "Failed to add patch to 3d object (id = %d)!!", name);
		return 0;
	}
	
	return name;
}

Surface3D * const graphics_get_surface3d(GraphicsSystem * const restrict this, const Name name) {
	/**
	 * Get a pointer to the surface object indentified by `name`.
	 */
	
	size_t idx = graphics_find_surface3d(this, name);
	
	if (idx == -1) {
		return NULL;
	}
	
	return &this->surface[idx];
}

size_t graphics_get_surface3d_counts(const GraphicsSystem * const restrict this, size_t *allocsz) {
	/**
	 * Return the count of surface3D objects and write the count of memory
	 * allocated to allocsz if not NULL.
	 */
	
	if (allocsz) {
		*allocsz = this->surface_alloc;
	}
	
	return this->surface_count;
}

/**
 * =============================================================================
 * 2D Meshes
 * =============================================================================
 */

static int graphics_realloc_mesh2d(GraphicsSystem * restrict this) {
	/**
	 * Reallocate the list of mesh entities, making sure there is at least one 
	 * free slot available. Returns non-zero when there is an error.
	 */
	
	if (this->mesh2d_count >= this->mesh2d_alloc) {
		this->mesh2d_alloc = 4 + (this->mesh2d_alloc * 2);
		this->mesh2d = DgRealloc(this->mesh2d, sizeof *this->mesh2d * this->mesh2d_alloc);
		
		if (!this->mesh2d) {
			return 1;
		}
		
		this->mesh2d_name = DgRealloc(this->mesh2d_name, sizeof *this->mesh2d_name * this->mesh2d_alloc);
		
		if (!this->mesh2d_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graphics_find_mesh2d(GraphicsSystem * restrict this, const Name name) {
	/**
	 * Find a mesh2d's index given its name.
	 */
	
	for (size_t i = 0; i < this->mesh2d_count; i++) {
		if (this->mesh2d_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graphics_create_mesh2d(GraphicsSystem * restrict this, Name name) {
	/**
	 * Create a 2D mesh object.
	 */
	
	if (graphics_realloc_mesh2d(this)) {
		return 0;
	}
	
	this->mesh2d_name[this->mesh2d_count] = name;
	memset(&this->mesh2d[this->mesh2d_count], 0, sizeof *this->mesh2d);
	
	this->mesh2d_count++;
	
	return name;
}

Name graphics_set_mesh2d(GraphicsSystem * restrict this, Name name, uint32_t vertex_count, QRVertex2D *vertex, uint32_t index_count, uint32_t *index, const char *texture) {
	/**
	 * Set the mesh data for the given 2D mesh entity.
	 */
	
	size_t i = graphics_find_mesh2d(this, name);
	
	if (i == -1) {
		return 0;
	}
	
	Mesh2D *mesh = &this->mesh2d[i];
	
	mesh->vertex = vertex;
	mesh->vertex_count = vertex_count;
	mesh->index = index;
	mesh->index_count = index_count;
	mesh->texture = texture ? DgStrdup(texture) : NULL;
	mesh->updated = true;
	
	return name;
}

Mesh2D * const graphics_get_mesh2d(GraphicsSystem * restrict this, Name name) {
	/**
	 * Retrieve a pointer to the mesh data structure.
	 */
	
	size_t i = graphics_find_mesh2d(this, name);
	
	if (i == -1) {
		return NULL;
	}
	
	return &this->mesh2d[i];
}

size_t graphics_get_mesh2d_counts(GraphicsSystem * restrict this, size_t *allocsz) {
	/**
	 * Get the count of 2d meshes that are active and optionally the count of 2d
	 * meshes that are allocated.
	 */
	
	if (allocsz) {
		*allocsz = this->mesh2d_alloc;
	}
	
	return this->mesh2d_count;
}

#undef GL_ERROR_CHECK
#undef ARRAY2_COL
#undef ARRAY2X_COL
#undef ARRAY2XX_COL
