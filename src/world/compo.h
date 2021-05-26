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
	// ================== //
	DgVec3 pos;
	DgVec3 rot;
	DgVec3 scale;
} CTransform;

typedef struct {
	ComponentBase_t base;
	// ================== //
	float * vert;
	uint32_t * index;
	uint32_t vert_count;
	uint32_t index_count;
	uint32_t vbo, ebo, vao;
	bool updated;
} CMesh;

typedef struct {
	ComponentBase_t base;
	// ================== //
	enum {
		QR_CAM_MOVE = 1,
		QR_CAM_ORTHO = 2,
		QR_CAM_EDITOR = 3,
		QR_CAM_BASE = 4,
	} mode;
} CCamera;

enum {
	// For writing state
	QR_PHYS_DISABLE_GRAVITY = (1<<0),
	QR_PHYS_ENABLE_RESPONSE = (1<<2),
	
	// For reading state
	QR_PHYS_GROUNDED = (1<<16),
};

typedef struct {
	ComponentBase_t base;
	// ================== //
	float mass;
	uint32_t flags;
	DgVec3 Vpos;
	DgVec3 Fpos;
	DgVec3 Vrot;
	DgVec3 Frot;
	uint32_t col_object;
} CPhysics;

typedef struct C_UIBox {
	ComponentBase_t base;
	// ================== //
	DgVec2 pos;
	DgVec2 size;
	uint32_t flags;
} C_UIBox;

typedef struct C_UIText {
	ComponentBase_t base;
	// ================== //
	const char *text;
	DgVec2 pos;
	float size;
	DgVec4 *vertex;
	uint32_t vertex_count;
	uint32_t *index;
	uint32_t index_count;
	uint32_t vbo, ebo, vao;
	bool updated;
} C_UIText;

typedef struct C_UIButton {
	ComponentBase_t base;
	// ================== //
	C_UIBox *box;
	C_UIText *text;
} C_UIButton;
