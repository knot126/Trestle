/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "../world/world.h"
#include "../util/alloc.h"
#include "../util/fail.h"
#include "../util/xml.h"

#include "world.h"

void world_init(World *world, size_t prealloc_count) {
	/*
	 * Creates a world full of entities
	 */
	memset(world, 0, sizeof(World));
	
	if (prealloc_count == 0) {
		return;
	}
	
	// prealloc is disabled for now...
}

uint32_t world_create_entity(World *world, mask_t mask) {
	/*
	 * Create a new entity in a given world and return its ID
	 */
	world->mask_count++;
	
	// NOTE: nullptr passed to realloc works like alloc, so we do not need to
	// consider a case where the list has not been initialised.
	world->mask = DgRealloc(world->mask, sizeof(mask_t) * world->mask_count);
	
	if (!world->mask) {
		DgFail("Allocation error: world->mask.\n", 403);
	}
	
	world->mask[world->mask_count - 1] = mask;
	
	// Allocate the nessicary components
	
	// Transform
	if ((mask & QR_COMPONENT_TRANSFORM) == QR_COMPONENT_TRANSFORM) {
		world->CTransforms_count++;
		world->CTransforms = DgRealloc(world->CTransforms, sizeof(CTransform) * world->CTransforms_count);
		
		if (!world->CTransforms) {
			DgFail("Allocation error: world->CTransforms.\n", 400);
		}
		
		memset((world->CTransforms + (world->CTransforms_count - 1)), 0, sizeof(CTransform));
		world->CTransforms[world->CTransforms_count - 1].base.id = world->mask_count;
	}
	
	// Mesh
	if ((mask & QR_COMPONENT_MESH) == QR_COMPONENT_MESH) {
		world->CMeshs_count++;
		world->CMeshs = DgRealloc(world->CMeshs, sizeof(CMesh) * world->CMeshs_count);
		
		if (!world->CMeshs) {
			DgFail("Allocation error: world->CMeshs.\n", 401);
		}
		
		memset((world->CMeshs + (world->CMeshs_count - 1)), 0, sizeof(CMesh));
		world->CMeshs[world->CMeshs_count - 1].base.id = world->mask_count;
	}
	
	// Camera
	if ((mask & QR_COMPONENT_CAMERA) == QR_COMPONENT_CAMERA) {
		world->CCameras_count++;
		world->CCameras = DgRealloc(world->CCameras, sizeof(CCamera) * world->CCameras_count);
		
		if (!world->CCameras) {
			DgFail("Allocation error: world->CCameras\n", 402);
		}
		
		memset((world->CCameras + (world->CCameras_count - 1)), 0, sizeof(CCamera));
		world->CCameras[world->CCameras_count - 1].base.id = world->mask_count;
	}
	
	return world->mask_count;
}

bool scene_load_xml(World *world, const char * const path) {
	DgXMLNode node;
	
	if (DgXMLLoad(&node, path)) {
		return false;
	}
	
	// for each entity tag
	for (uint32_t s = 0; s < node.sub_count; s++) {
		if (!strcmp(node.sub[s].name, "entity")) {
			
			// for each tag in the entity tag
			for (uint32_t t = 0; t < node.sub[s].sub_count; t++) {
				if (!strcmp(node.sub[s].sub[t].name, "transform")) {
					
				}
			}
		}
	}
	
	return true;
}
