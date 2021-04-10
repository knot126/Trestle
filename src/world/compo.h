/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Entity-Component-Systems
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "../util/maths.h"

typedef uint32_t EntityId_t;

typedef struct {
	EntityId_t id;
} ComponentBase_t;

typedef struct {
	ComponentBase_t base;
	DgVec3 pos;
	DgVec3 rot;
} CTransform;

typedef struct {
	ComponentBase_t base;
	bool updated; // Putting this here *should* avoid wasted space due to padding.
	float * vert;
	uint32_t * index;
	uint32_t vert_count;
	uint32_t index_count;
	uint32_t vbo, ebo;
} CMesh;

typedef struct {
	ComponentBase_t base;
	
} CCamera;
