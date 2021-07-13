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

#include "physics.h"

void physics_init(PhysicsSystem *this) {
	/**
	 * Initialise a physics system.
	 */
	
	memset(this, 0, sizeof *this);
}

static void physics_update_gravity(PhysicsSystem *this, SceneGraph *graph) {
	/**
	 * Update gravity on all objects.
	 */
	
	for (size_t i = 0; i < this->object_count; i++) {
		Transform * const trans = graph_get(graph, this->object_name[i]);
		PhysicsObject * const obj = &this->object[i];
		
		if ((obj->flags & PHYSICS_STATIC) != PHYSICS_STATIC) {
			trans->pos.y -= 0.001f;
		}
	}
}

void physics_update(PhysicsSystem *this, SceneGraph *graph) {
	/**
	 * Update the physics system and advance the simulation.
	 */
	
	physics_update_gravity(this, graph);
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
