/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */

#include <inttypes.h>
#include <string.h>

#include "util/maths.h"

#include "physics.h"

void physics_init(PhysicsSystem *this) {
	/**
	 * Initialise a physics system.
	 */
	
	memset(this, 0, sizeof *this);
}

void physics_update(PhysicsSystem *this) {
	/**
	 * Update the physics system and advance the simulation.
	 */
	
}

void physics_free(PhysicsSystem *this) {
	/**
	 * Frees all resources used by the physics system.
	 */
	
}
