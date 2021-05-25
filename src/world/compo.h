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
	float * vert;
	uint32_t * index;
	uint32_t vert_count;
	uint32_t index_count;
	uint32_t vbo, ebo, vao;
	bool updated;
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
	// For writing state
	QR_PHYS_DISABLE_GRAVITY = (1<<0),
	QR_PHYS_ENABLE_RESPONSE = (1<<2),
	
	// For reading state
	QR_PHYS_GROUNDED = (1<<16),
};

typedef struct {
	ComponentBase_t base;
	float mass;
	uint32_t flags;
	DgVec3 Vpos;
	DgVec3 Fpos;
	DgVec3 Vrot;
	DgVec3 Frot;
	uint32_t col_object;
} CPhysics;

typedef struct SC_UIBox {
	ComponentBase_t base;
	DgVec2 pos;
	DgVec2 size;
	uint32_t flags;
} SC_UIBox;

typedef struct SC_UIText {
	ComponentBase_t base;
	const char *text;
	DgVec2 pos;
	float size;
	uint8_t align_h;
	uint8_t align_v;
	uint16_t font;
	
	float vertex_cache;
	uint32_t vertex_count;
} SC_UIText;

typedef struct C_UIButton {
	ComponentBase_t base;
	SC_UIBox *box;
	SC_UIText *text;
} C_UIButton;
