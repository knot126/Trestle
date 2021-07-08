/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */

#include <inttypes.h>
#include <string.h>

#include "util/maths.h"
#include "world/compo.h"
#include "world/world.h"

#include "physics.h"

void physics_new(PhysicsSystem *this) {
	/**
	 * Initialise a physics system.
	 */
	
	memset(this, 0, sizeof *this);
}

void physics_register_world(PhysicsSystem *this, World *world) {
	/**
	 * Regiser a legacy world for the physics system to interoperate with.
	 */
	
	this->world = world;
}

void physics_update(PhysicsSystem *this) {
	/**
	 * Update the physics system and advance the simulation.
	 */
	
}
