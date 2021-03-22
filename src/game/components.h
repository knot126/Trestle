/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Components
 */

#pragma

#include <inttypes.h>

#include "../utils/math.h"

typedef enum ComponentType {
	QR_COMP_NULL,
	QR_COMP_TRANSFORM,
	QR_COMP_MESH_DATA,
	QR_COMP_PHYSICS_BODY,
	QR_COMP_PROPERTIES,
	QR_COMP_CAMERA,
	QR_COMP_PLAYER,
} ComponentType;

typedef struct ComponentBase {
	uint64_t type;
} ComponentBase;

typedef struct CTransform {
	ComponentBase _base;
	DgVec3 position;
	DgVec3 rotation;
	DgVec3 scale;
} CTransform;
