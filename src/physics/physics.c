/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */

#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "graph/graph.h"
#include "util/maths.h"
#include "util/alloc.h"
#include "util/time.h"
#include "util/log.h"

#include "physics.h"

void physics_init(PhysicsSystem *this) {
	/**
	 * Initialise a physics system.
	 */
	
	memset(this, 0, sizeof *this);
	
	this->gravity = (DgVec3) {0.0f, -9.81f, 0.0f};
}

static void reset_forces(PhysicsSystem *this, SceneGraph *graph, float delta) {
	/**
	 * Reset forces on all objects to their default. (0, 0, 0) with no gravity, 
	 * or whatever gravity is with that enabled.
	 */
	
	if (delta == 0.0f) {
		return;
	}
	
	for (size_t i = 0; i < this->object_count; i++) {
		Transform * const trans = graph_get(graph, this->object_name[i]);
		PhysicsObject * const obj = &this->object[i];
		
		// Reset forces to zero
		obj->accel = (DgVec3) {0.0f, 0.0f, 0.0f};
		
		// Gravity
		if ((obj->flags & PHYSICS_NO_GRAVITY) != PHYSICS_NO_GRAVITY && (obj->flags & PHYSICS_STATIC) != PHYSICS_STATIC) {
			obj->accel = DgVec3Add(obj->accel, this->gravity);
		}
		
		// Drag
		// NOTE: This doesn't work (it's not the correct thing to do, just a
		// dummy algorithm)
		if ((obj->flags & PHYSICS_DRAG) == PHYSICS_DRAG && (obj->flags & PHYSICS_STATIC) != PHYSICS_STATIC) {
			obj->accel = DgVec3Subtract(obj->accel, DgVec3Scale(0.02f, obj->accel));
		}
	}
}

static bool Test_AABB_AABB(AABBShape *a, AABBShape *b) {
	/**
	 * Check a collision between two AABB shapes.
	 */
	
	DgVec3 amin = DgVec3Add(a->pos, a->size);
	DgVec3 amax = DgVec3Subtract(a->pos, a->size);
	DgVec3 bmin = DgVec3Add(b->pos, b->size);
	DgVec3 bmax = DgVec3Subtract(b->pos, b->size);
	
	return ((amin.x >= bmax.x) && (bmin.x >= amax.x))
		&& ((amin.y >= bmax.y) && (bmin.y >= amax.y))
		&& ((amin.z >= bmax.z) && (bmin.z >= amax.z));
}

static float flt_abs_phys(float A) { return (A < 0.0f) ? -A : A; }

static void Resolve_AABB_AABB(PhysicsSystem *this, SceneGraph *graph, size_t i, size_t j) {
	/**
	 * Respond to a collision between two AABB objects.
	 * 
	 * We will a assume that I (A) should be moved out of J (B)'s way.
	 * 
	 * WARNING: This will be replaced once I figure out how to do this properly.
	 */
	
	DgVec3 amin = DgVec3Add(this->aabb[i].pos, this->aabb[i].size);
	DgVec3 amax = DgVec3Subtract(this->aabb[i].pos, this->aabb[i].size);
	DgVec3 bmin = DgVec3Add(this->aabb[j].pos, this->aabb[j].size);
	DgVec3 bmax = DgVec3Subtract(this->aabb[j].pos, this->aabb[j].size);
	
	// Get the transform
	Transform * const trans = graph_get(graph, this->aabb_name[i]);
	
	if (!trans) {
		return;
	}
	
	// Find the difference
	DgVec3 diff = DgVec3Subtract(amax, bmin);
	
// 	printf("(%f, %f, %f)\n", diff.x, diff.y, diff.z);
	
	// Find out which penertates the least and push based on that.
	if (diff.x > 0.0f && diff.y > diff.x && diff.z > diff.x) {
		trans->pos.x += diff.x;
	}
	
	if (diff.x < 0.0f && diff.y < diff.x && diff.z < diff.x) {
		trans->pos.x -= diff.x;
	}
	
	if (diff.y > 0.0f && diff.x > diff.y && diff.z > diff.y) {
		trans->pos.y += diff.y;
	}
	
	if (diff.y < 0.0f && diff.x < diff.y && diff.z < diff.y) {
		trans->pos.y -= diff.y;
	}
	
	if (diff.z > 0.0f && diff.x > diff.z && diff.y > diff.z) {
		trans->pos.z += diff.z;
	}
	
	if (diff.z < 0.0f && diff.x < diff.z && diff.y < diff.z) {
		trans->pos.z -= diff.z;
	}
}

static size_t physics_find_object(PhysicsSystem *this, Name name);

static void resolve_collisions(PhysicsSystem *this, SceneGraph *graph, float delta) {
	/**
	 * Check and resolve all collisions 
	 */
	
	// Fix AABB positions
	for (size_t i = 0; i < this->aabb_count; i++) {
		this->aabb[i].pos = graph_get(graph, this->aabb_name[i])->pos;
	}
	
	// AABB to AABB
	for (size_t i = 0; i < this->aabb_count; i++) {
		for (size_t j = 0; j < this->aabb_count; j++) {
			if (i != j 
				&& Test_AABB_AABB(&this->aabb[i], &this->aabb[j]) 
				&& (this->object[physics_find_object(this, this->aabb_name[i])].flags & PHYSICS_STATIC) != PHYSICS_STATIC) {
// 				DgLog(DG_LOG_VERBOSE, "(i = %d, j = %d) Colliding!!", i, j);
				Resolve_AABB_AABB(this, graph, i, j);
			}
		}
	}
}

static void update_gravity(PhysicsSystem *this, SceneGraph *graph, float delta) {
	/**
	 * Update gravity on all objects.
	 */
	
	if (delta == 0.0f) {
		return;
	}
	
	for (size_t i = 0; i < this->object_count; i++) {
		Transform * const trans = graph_get(graph, this->object_name[i]);
		PhysicsObject * const obj = &this->object[i];
		
		// Apply forces to the object
		if ((obj->flags & PHYSICS_STATIC) != PHYSICS_STATIC) {
			// Apply direct forces in player mode
			if ((obj->flags & PHYSICS_MODE_PLAYER) == PHYSICS_MODE_PLAYER) {
				trans->pos = DgVec3Add(trans->pos, DgVec3Scale(delta, obj->directForce));
				obj->lastPos = DgVec3Add(obj->lastPos, DgVec3Scale(delta, obj->directForce));
				obj->directForce = (DgVec3) {0.0f, 0.0f, 0.0f};
			}
			
			// Integrate forces a normal
			const DgVec3 tempold = obj->lastPos;
			const DgVec3 tempcur = trans->pos;
			trans->pos = DgVec3Add(
				DgVec3Add(tempcur, DgVec3Subtract(tempcur, tempold)),
				DgVec3Scale(delta, DgVec3Scale(delta, obj->accel))
			);
			obj->lastPos = tempcur;
		}
	}
}

void physics_update(PhysicsSystem *this, SceneGraph *graph, float delta) {
	/**
	 * Update the physics system and advance the simulation.
	 */
	
	// Check if physics are currently enabled
	if (!this->enabled) {
		return;
	}
	
	update_gravity(this, graph, delta);
	resolve_collisions(this, graph, delta);
	reset_forces(this, graph, delta);
}

void physics_free(PhysicsSystem *this) {
	/**
	 * Frees all resources used by the physics system.
	 */
	
	if (this->object_name) {
		DgFree(this->object_name);
	}
	
	if (this->object) {
		DgFree(this->object);
	}
	
	if (this->aabb_name) {
		DgFree(this->aabb_name);
	}
	
	if (this->aabb) {
		DgFree(this->aabb);
	}
}

void physics_enabled(PhysicsSystem *this, bool mode) {
	/**
	 * Set physics as enabled or disabled.
	 */
	
	this->enabled = mode;
}

static uint8_t physics_reallocate(PhysicsSystem *this) {
	/**
	 * Automatically reallocate physics feilds as needed.
	 */
	
	// Physics Objects
	if (this->object_count == this->object_alloc) {
		this->object_alloc = 2 + (this->object_alloc * 2);
		
		this->object_name = DgRealloc(this->object_name, sizeof *this->object_name * this->object_alloc);
		
		if (!this->object_name) {
			return 1;
		}
		
		this->object = DgRealloc(this->object, sizeof *this->object * this->object_alloc);
		
		if (!this->object_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t physics_find_object(PhysicsSystem *this, Name name) {
	/**
	 * Find the index for the given object name.
	 */
	
	for (size_t i = 0; i < this->object_count; i++) {
		if (this->object_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name physics_create_object(PhysicsSystem *this, Name name) {
	/**
	 * Create a physics object by the given name.
	 */
	
	if (physics_reallocate(this)) {
		return 0;
	}
	
	this->object_count++;
	
	this->object_name[this->object_count - 1] = name;
	
	return name;
}

Name physics_clear_object(PhysicsSystem *this, Name name) {
	/**
	 * Clear a physics object's data.
	 */
	
	size_t index = physics_find_object(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->object[index].lastPos = (DgVec3) {0.0f, 0.0f, 0.0f};
	this->object[index].accel = (DgVec3) {0.0f, 0.0f, 0.0f};
	this->object[index].flags = 0;
	
	return name;
}

Name physics_set_accel(PhysicsSystem *this, Name name, DgVec3 accel) {
	/**
	 * Set an object's acceleration data.
	 */
	
	size_t index = physics_find_object(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->object[index].accel = accel;
	
	return name;
}

Name physics_set_flags(PhysicsSystem *this, Name name, uint64_t flags) {
	/**
	 * Set an object's acceleration data.
	 */
	
	size_t index = physics_find_object(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->object[index].flags = flags;
	
	return name;
}

Name physics_add_forces(PhysicsSystem *this, Name name, DgVec3 force) {
	/**
	 * Add a force to an object for one physics frame.
	 */
	
	size_t index = physics_find_object(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->object[index].accel = DgVec3Add(this->object[index].accel, force);
	
	return name;
}

Name physics_move_object(PhysicsSystem *this, Name name, DgVec3 force) {
	/**
	 * Moves an object directly (only in player mode)
	 */
	
	size_t index = physics_find_object(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->object[index].directForce = DgVec3Add(this->object[index].directForce, force);
	
	return name;
}

void physics_sync_graph(PhysicsSystem *this, SceneGraph *graph) {
	/**
	 * Sync a physics system's object positions with the graphs object
	 * positions.
	 */
	
	for (size_t i = 0; i < this->object_count; i++) {
		const Name name = this->object_name[i];
		const Transform * const trans = graph_get(graph, name);
		PhysicsObject * const phys = &this->object[i];
		
		phys->lastPos = trans->pos;
		phys->accel = (DgVec3) {0.0f, 0.0f, 0.0f};
	}
}

/**
 * =============================================================================
 * AABB Shapes
 * =============================================================================
 */

static uint8_t aabb_reallocate(PhysicsSystem *this) {
	/**
	 * Automatically reallocate physics feilds as needed.
	 */
	
	// Physics Objects
	if (this->aabb_count == this->aabb_alloc) {
		this->aabb_alloc = 2 + (this->aabb_alloc * 2);
		
		this->aabb_name = DgRealloc(this->aabb_name, sizeof *this->aabb_name * this->aabb_alloc);
		
		if (!this->aabb_name) {
			return 1;
		}
		
		this->aabb = DgRealloc(this->aabb, sizeof *this->aabb * this->aabb_alloc);
		
		if (!this->aabb_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t physics_find_aabb(PhysicsSystem *this, Name name) {
	/**
	 * Find the index for the given aabb name.
	 */
	
	for (size_t i = 0; i < this->aabb_count; i++) {
		if (this->aabb_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name physics_create_aabb(PhysicsSystem *this, Name name) {
	/**
	 * Create a physics aabb by the given name.
	 */
	
	if (aabb_reallocate(this)) {
		return 0;
	}
	
	this->aabb_count++;
	
	this->aabb_name[this->aabb_count - 1] = name;
	
	return name;
}

Name physics_set_aabb(PhysicsSystem *this, Name name, DgVec3 size) {
	/**
	 * Set an AABB's shape.
	 */
	
	size_t index = physics_find_aabb(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->aabb[index].size = size;
	
	return name;
}
