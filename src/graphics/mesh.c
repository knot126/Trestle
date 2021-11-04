/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh representation and tools
 */

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "graphics/graphics.h"
#include "graphics/vertex3d.h"
#include "util/fs.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/rand.h"
#include "util/log.h"
#include "util/str.h"
#include "util/stream.h"
#include "util/obj.h"

#include "mesh.h"

bool graphics_load_obj_mesh(GraphicsSystem * restrict this, Name name, const char * const restrict path) {
	/**
	 * Load an XML mesh
	 */
	
	// Find the mesh component
	Mesh *mesh = graphics_get_mesh(this, name);
	
	if (!mesh) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh or model '%s' to entity %d: mesh not found.", path, name);
		return false;
	}
	
	DgOBJMesh obj;
	
	uint32_t status = DgOBJLoad(&obj, (char *) path);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh or model '%s' to entity %d: failed to load OBJ.", path, name);
		return false;
	}
	
	QRVertex1 *vert = (QRVertex1 *) DgAlloc(sizeof *vert * obj.vertex_count);
	
	if (!vert) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh or model '%s' to entity %d: failed to allocate memory.", path, name);
		DgOBJFree(&obj);
		return false;
	}
	
	for (size_t i = 0; i < obj.vertex_count; i++) {
		vert[i].x = obj.vertex[i].x;
		vert[i].y = obj.vertex[i].y;
		vert[i].z = obj.vertex[i].z;
		
		vert[i].u = 0.0f;
		vert[i].v = 0.0f;
		
		vert[i].r = DgRandFloat();
		vert[i].g = DgRandFloat();
		vert[i].b = DgRandFloat();
	}
	
	mesh->vert = (float *) vert;
	mesh->vert_count = obj.vertex_count;
	mesh->index = obj.face;
	mesh->index_count = obj.face_count;
	
	mesh->updated = true;
	
	DgFree(obj.vertex);
	
	return true;
}
