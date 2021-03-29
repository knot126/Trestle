/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "../generic/world.h"
#include "../util/alloc.h"

void world_init(World *world, size_t prealloc_count) {
	/*
	 * Creates a world full of entities
	 */
	memset(world, 0, sizeof(World));
	
	if (prealloc_count == 0) {
		return;
	}
	
	// Pre-allocate components to a count
	world->mask = DgAlloc(prealloc_count * sizeof(EntityComponentMask_t));
	world->mask_alloc = prealloc_count;
	
	world->CTransforms = DgAlloc(prealloc_count * sizeof(CTransform));
	world->CTransforms_alloc = prealloc_count;
	
	world->CMeshs = DgAlloc(prealloc_count * sizeof(CMesh));
	world->CMeshs_alloc = prealloc_count;
}

uint32_t world_create_entity(World *world, EntityComponentMask_t mask) {
	/*
	 * Create a new entity in a given world
	 */
	
}
