/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#pragma once

#include <inttypes.h>

#include "../generic/compo.h"

typedef uint32_t EntityComponentMask_t;

typedef enum {
	QR_COMPONENT_TRANSFORM = 1 << 0,
	QR_COMPONENT_MESH = 1 << 1,
} ComponentMaskEnum;

typedef struct {
	// Masks and entities
	EntityComponentMask_t *mask;
	size_t count;
	
	// Transform Components
	CTransform *CTransfroms;
	size_t      CTransfroms_count;
	size_t      CTransfroms_alloc;
	
	// Mesh Components
	CMesh *CMeshs;
	size_t CMeshs_count;
	size_t CMeshs_alloc;
} World;

void world_init(World *world, size_t prealloc_count);
