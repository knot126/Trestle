/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "../world/compo.h"

typedef uint32_t mask_t;

typedef enum {
	QR_COMPONENT_TRANSFORM = 1 << 0,
	QR_COMPONENT_MESH = 1 << 1,
	QR_COMPONENT_CAMERA = 1 << 2,
	QR_COMPONENT_PHYSICS = 1 << 3,
} ComponentMaskEnum;

typedef struct {
	// Masks and entities
	mask_t  *mask;
	uint32_t mask_count;
	uint32_t mask_alloc;
	
	// Transform Components
	CTransform *CTransforms;
	uint32_t    CTransforms_count;
	uint32_t    CTransforms_alloc;
	
	// Mesh Components
	CMesh   *CMeshs;
	uint32_t CMeshs_count;
	uint32_t CMeshs_alloc;
	
	// Camera Components
	CCamera *CCameras;
	uint32_t CCameras_count;
	uint32_t CCameras_alloc;
	uint32_t CCameras_active[2];
	
	// Physics Components
	CPhysics *CPhysicss;
	uint32_t  CPhysicss_count;
	uint32_t  CPhysicss_alloc;
	
} World;

void world_init(World *world, size_t prealloc_count);
void world_destroy(World *world);
uint32_t world_create_entity(World *world, mask_t mask);
