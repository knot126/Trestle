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
	DgVec3 scale;
} CTransform;

typedef struct {
	ComponentBase_t base;
	bool updated; // Putting this here *should* avoid wasted space due to padding. (NO!)
	float * vert;
	uint32_t * index;
	uint32_t vert_count;
	uint32_t index_count;
	uint32_t vbo, ebo;
} CMesh;

typedef struct {
	ComponentBase_t base;
	enum {
		QR_CAM_MOVE = 1,
		QR_CAM_ORTHO = 2,
		QR_CAM_EDITOR = 3,
		QR_CAM_BASE = 4,
	} mode;
} CCamera;

enum {
	QR_PHYS_DISABLE_GRAVITY = (1<<0),
	QR_PHYS_ENABLE_COLLISION_RESPONSE = (1<<1),
	QR_PHYS_DISABLE_COLLISION = (1<<2),
};

typedef struct {
	ComponentBase_t base;
	float mass;
	int flags;
	DgVec3 Vpos;
	DgVec3 Fpos;
	DgVec3 Vrot;
	DgVec3 Frot;
	uint32_t col_object;
} CPhysics;

typedef struct {
	DgVec3 pos;
	DgVec3 size;
} CShapeAABB;
