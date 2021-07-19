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
	PHYSICS_NO_GRAVITY = (1 << 1),
	PHYSICS_DRAG = (1 << 2),
	PHYSICS_MODE_PLAYER = (1 << 3), // uses a different integration scheme
};

typedef struct PhysicsObject {
	DgVec3 lastPos;
	DgVec3 accel;
	uint64_t flags;
} PhysicsObject;

typedef struct AABBShape {
	DgVec3 pos;
	DgVec3 size;
} AABBShape;

typedef struct SphereShape {
	DgVec3 pos;
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
	
	/// Globals ///
	DgVec3 gravity;
	float delta_time;
	bool enabled;
} PhysicsSystem;

void physics_init(PhysicsSystem *this);
void physics_update(PhysicsSystem *this, SceneGraph *graph, float delta);
void physics_free(PhysicsSystem *this);
void physics_enabled(PhysicsSystem *this, bool mode);

Name physics_create_object(PhysicsSystem *this, Name name);
Name physics_clear_object(PhysicsSystem *this, Name name);
Name physics_set_accel(PhysicsSystem *this, Name name, DgVec3 accel);
Name physics_set_flags(PhysicsSystem *this, Name name, uint64_t flags);
Name physics_add_forces(PhysicsSystem *this, Name name, DgVec3 force);

void physics_sync_graph(PhysicsSystem *this, SceneGraph *graph);

Name physics_create_aabb(PhysicsSystem *this, Name name);
Name physics_set_aabb(PhysicsSystem *this, Name name, DgVec3 size);
