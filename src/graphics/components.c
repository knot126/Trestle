/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics-system related functions
 */

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../generic/world.h"
#include "../generic/compo.h"
#include "../io/fs.h"
#include "../util/alloc.h"

#include "components.h"

bool entity_load_mesh(World *world, uint32_t id, char *path) {
	/*
	 * Loads an uncompressed mesh from a file into an entity's mesh component
	 */
	
	// Find the mesh component
	CMesh *mesh = NULL;
	
	for (int i = 0; i < world->CMeshs_count; i++) {
		if (world->CMeshs[i].base.id == id) {
			mesh = &world->CMeshs[i];
			break;
		}
	}
	
	if (!mesh) {
		printf("Failed to load mesh or model '%s' to entity %d.\n", path, id);
		return false;
	}
	
	// Open the file stream
	char *real_path = DgEvalPath(path);
	DgFileStream *s = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!s) {
		printf("Failed to load file '%s'.\n", path);
		return false;
	}
	
	// Read vertexes
	DgFileStreamReadInt32(s, &mesh->vert_count);
	mesh->vert = DgAlloc(mesh->vert_count * 32);
	if (!mesh->vert) {
		printf("Failed to allocate memory whilst trying to load a mesh file.\n");
		return false;
	}
	DgFileStreamRead(s, mesh->vert_count * 32, mesh->vert);
	
	// Read indexes
	DgFileStreamReadInt32(s, &mesh->index_count);
	mesh->index = DgAlloc(mesh->index_count * 4);
	if (!mesh->index) {
		printf("Failed to allocate memory whilst trying to load a mesh file.\n");
		return false;
	}
	DgFileStreamRead(s, mesh->index_count * 4, mesh->index);
	
	mesh->updated = true;
	
	DgFileStreamClose(s);
	
	return true;
}