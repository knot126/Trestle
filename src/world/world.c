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

World *QuickRunActiveWorld;

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

void world_destroy(World *world) {
	/* 
	 * Frees the a world and its exsiting resources
	 */
	
	if (world->mask) {
		DgFree(world->mask);
	}
	
	if (world->CTransforms) {
		DgFree(world->CTransforms);
	}
	
	if (world->CMeshs) {
		DgFree(world->CMeshs);
	}
	
	if (world->CCameras) {
		DgFree(world->CCameras);
	}
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
	
	// Physics
	if ((mask & QR_COMPONENT_PHYSICS) == QR_COMPONENT_PHYSICS) {
		world->CPhysicss_count++;
		world->CPhysicss = DgRealloc(world->CPhysicss, sizeof(CPhysics) * world->CPhysicss_count);
		
		if (!world->CPhysicss) {
			DgFail("Allocation error: world->CPhysicss\n", 402);
		}
		
		memset((world->CPhysicss + (world->CPhysicss_count - 1)), 0, sizeof(CPhysics));
		world->CPhysicss[world->CPhysicss_count - 1].base.id = world->mask_count;
	}
	
	return world->mask_count;
}

void SetActiveWorld(World *world) {
	QuickRunActiveWorld = world;
}
