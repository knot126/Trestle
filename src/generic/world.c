/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "../generic/world.h"
#include "../util/alloc.h"
#include "../util/fail.h"

#include "world.h"

void world_init(World *world, size_t prealloc_count) {
	/*
	 * Creates a world full of entities
	 */
	memset(world, 0, sizeof(World));
	
	if (prealloc_count == 0) {
		return;
	}
	
	// Pre-allocate components to a count
	/* NOTE: Disabled for now to make things simplier. I am going to look more
	 * into methods of managing C arrays soon and these will be re-enabled if I
	 * think it is optimal to do so.
	world->mask = DgAlloc(prealloc_count * sizeof(mask_t));
	world->mask_alloc = prealloc_count;
	
	world->CTransforms = DgAlloc(prealloc_count * sizeof(CTransform));
	world->CTransforms_alloc = prealloc_count;
	
	world->CMeshs = DgAlloc(prealloc_count * sizeof(CMesh));
	world->CMeshs_alloc = prealloc_count;
	*/
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
		DgFail("Allocation error: world->mask.\n", 3);
	}
	
	world->mask[world->mask_count - 1] = mask;
	
	// Allocate the nessicary components
	
	// Transform
	if (mask & QR_COMPONENT_TRANSFORM == QR_COMPONENT_TRANSFORM) {
		world->CTransforms_count++;
		world->CTransforms = DgRealloc(world->CTransforms, sizeof(CTransform) * world->CTransforms_count);
		
		if (!world->CTransforms) {
			DgFail("Allocation error: world->CTransforms.\n", 3);
		}
		
		memset((world->CTransforms + (world->CTransforms_count - 1)), 0, sizeof(CTransform));
		world->CTransforms[world->CTransforms_count - 1].base.id = world->mask_count;
	}
	
	// Mesh
	if (mask & QR_COMPONENT_MESH == QR_COMPONENT_MESH) {
		world->CMeshs_count++;
		world->CMeshs = DgRealloc(world->CMeshs, sizeof(CMesh) * world->CMeshs_count);
		
		if (!world->CTransforms) {
			DgFail("Allocation error: world->CMeshs.\n", 3);
		}
		
		memset((world->CMeshs + (world->CMeshs_count - 1)), 0, sizeof(CMesh));
		world->CMeshs[world->CMeshs_count - 1].base.id = world->mask_count;
	}
	
	return world->mask_count;
}
