/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */

#include <inttypes.h>
#include <string.h>

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

static void physics_prepare_forces(PhysicsSystem *this, SceneGraph *graph, float delta) {
	/**
	 * Prepares forces on all objects for being updated by the physics system.
	 * Mainly of note, this applies the delta time to the forces.
	 */
	
	if (delta == 0.0f) {
		return;
	}
	
	for (size_t i = 0; i < this->object_count; i++) {
		Transform * const trans = graph_get(graph, this->object_name[i]);
		PhysicsObject * const obj = &this->object[i];
		
		// We divide here to get m/s from m/s^2
		// In other words, we apply the delta time transformation for all forces
		// here.
// 		obj->accel = DgVec3Scale(1.0f / delta, obj->accel);
	}
}

static void physics_reset_forces(PhysicsSystem *this, SceneGraph *graph, float delta) {
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

static void physics_update_gravity(PhysicsSystem *this, SceneGraph *graph, float delta) {
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
	
	physics_prepare_forces(this, graph, delta);
	physics_update_gravity(this, graph, delta);
	physics_reset_forces(this, graph, delta);
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
