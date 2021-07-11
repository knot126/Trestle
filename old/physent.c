/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics entity helper functions
 */

#include "world/compo.h"
#include "world/world.h"
#include "util/maths.h"

#include "physent.h"

/**
 * -----------------------------------------------------------------------------
 * Physics component related functions
 * -----------------------------------------------------------------------------
 */

bool entity_phys_set_flags(World * const restrict world, const uint32_t id, const int flags) {
	C_Physics *phys = entity_find_phys(world, id);
	
	if (!phys) {
		return false;
	}
	
	phys->flags = flags;
	
	return true;
}

bool entity_phys_set_mass(World * const restrict world, const uint32_t id, const float mass) {
	C_Physics *phys = entity_find_phys(world, id);
	
	if (!phys) {
		return false;
	}
	
	phys->mass = mass;
	
	return true;
}

bool entity_phys_add_force(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot) {
	C_Physics *phys = entity_find_phys(world, id);
	
	if (!phys) {
		return false;
	}
	
	phys->Fpos = DgVec3Scale(phys->mass, pos);
	phys->Frot = DgVec3Scale(phys->mass, rot);
	
	return true;
}
