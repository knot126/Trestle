/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Entity, Component and Scene Types
 */

#pragma once

#include <inttypes.h>

#include "../utils/array.h"
#include "../utils/math.h"
#include "../utils/bag.h"

/*
 * Enumerations
 */

// Component types
enum {
	QR_COMP_NULL,
	QR_COMP_TRANSFORM,
	QR_COMP_MESH_DATA,
	QR_COMP_PHYSICS_BODY,
	QR_COMP_PROPERTIES,
	QR_COMP_CAMERA,
	QR_COMP_PLAYER,
};

// Kinds of physics bodies
typedef enum PhysicsBodyKind {
	QR_PHYS_STATICBODY,
	QR_PHYS_RIGIDBODY,
} PhysicsBodyKind;

/*
 * Base types (DO NOT MAKE INSTANCES OF THESE!!)
 */

typedef struct ComponentBase {
	uint64_t type;
} ComponentBase;

typedef struct Entity {
	uint64_t id;
	Array compos;
	Array children;
} Entity;

/*
 * Scene-related types
 */

typedef struct Scene {
	uint64_t id;
	Array entities;
	Enitity * active_camera;
} Scene;

/*
 * Component Structure 
 * 
 * These should start with ComponentBase since this would be like inheriting 
 * from a struct in C++ and every component should be a complete ComponentBase 
 * on its own.
 */

typedef struct CTransform {
	ComponentBase _base;
	DgVec3 position;
	DgVec3 rotation;
	DgVec3 scale;
} CTransform;

typedef struct CMeshData {
	ComponentBase _base;
	float * mesh;
} CRenderInfo;

typedef struct CPhysicsBody {
	ComponentBase _base;
	uint32_t type;
} CPhysicsBody;

typedef struct CProperties {
	ComponentBase _base;
	DgBag properties;
} CProperties;
