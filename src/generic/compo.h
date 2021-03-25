/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Entity-Component-Systems
 */

#pragma once

#include <inttypes.h>

#include "../util/maths.h"

typedef uint32_t EntityId_t;

typedef struct {
	EntityId_t id;
} ComponentBase_t;

typedef struct {
	ComponentBase_t _base;
	DgVec3 pos;
	DgVec3 rot;
} CTransform;

typedef struct {
	ComponentBase_t _base;
	float * mesh;
	int mesh_length;
} CMesh;
