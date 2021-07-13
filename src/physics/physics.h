/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */ 

#pragma once

#include <stddef.h>

#include "util/maths.h"

#include "types.h"

typedef struct AABBShape {
	DgVec3 size;
} AABBShape;

typedef struct SphereShape {
	float radius;
} SphereShape;

typedef struct PhysicsObject {
	uint64_t flags;
} PhysicsObject;

typedef struct PhysicsSystem {
	/// Physics Objects ///
	Name          *object_name;
	PhysicsObject *object;
	size_t         object_count;
	
	/// Axis Aligned Bounding Boxes ///
	Name      *aabb_name;   // Name of the object
	AABBShape *aabb;        // The shape of the object
	size_t     aabb_count;
	
	/// Spheres ///
	Name        *sphere_name;
	SphereShape *sphere;
	size_t       sphere_count;
} PhysicsSystem;

void physics_init(PhysicsSystem *this);
void physics_update(PhysicsSystem *this);
void physics_free(PhysicsSystem *this);
