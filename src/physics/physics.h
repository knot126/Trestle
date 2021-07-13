/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */ 

#pragma once

#include <stddef.h>

#include "graph/graph.h"
#include "util/maths.h"

#include "types.h"

enum {
	PHYSICS_STATIC = (1 << 0),
};

typedef struct PhysicsObject {
	DgVec3 lastPos;
	DgVec3 accel;
	uint64_t flags;
} PhysicsObject;

typedef struct AABBShape {
	DgVec3 size;
} AABBShape;

typedef struct SphereShape {
	float radius;
} SphereShape;

typedef struct PhysicsSystem {
	/// Physics Objects ///
	Name          *object_name;
	PhysicsObject *object;
	size_t         object_count;
	size_t         object_alloc;
	
	/// Axis Aligned Bounding Boxes ///
	Name      *aabb_name;
	AABBShape *aabb;
	size_t     aabb_count;
	size_t     aabb_alloc;
	
	/// Spheres ///
	Name        *sphere_name;
	SphereShape *sphere;
	size_t       sphere_count;
	size_t       sphere_alloc;
	
	float delta_time;
} PhysicsSystem;

void physics_init(PhysicsSystem *this);
void physics_update(PhysicsSystem *this, SceneGraph *graph);
void physics_free(PhysicsSystem *this);

Name physics_create_object(PhysicsSystem *this, Name name);
Name physics_clear_object(PhysicsSystem *this, Name name);
Name physics_set_accel(PhysicsSystem *this, Name name, DgVec3 accel);
Name physics_set_flags(PhysicsSystem *this, Name name, uint64_t flags);
