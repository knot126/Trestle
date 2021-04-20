/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Transfroms
 */

#include <stdbool.h>
#include <inttypes.h>

#include "../world/compo.h"
#include "../world/world.h"
#include "../util/maths.h"

bool entity_set_transform(World *world, uint32_t id, DgVec3 pos, DgVec3 rot, DgVec3 scale) {
	CTransform *trans = NULL;
	
	// Find the transfrom
	for (uint32_t i = 0; i < world->CTransforms_count; i++) {
		if (world->CTransforms[i].base.id == id) {
			trans = (world->CTransforms + i);
			break;
		}
	}
	
	if (!trans) {
		return false;
	}
	
	trans->pos = pos;
	trans->rot = rot;
	trans->scale = scale;
	
	return true;
}

bool entity_phys_set_flags(World *world, uint32_t id, int flags) {
	CPhysics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == id) {
			phys = (world->CPhysicss + i);
			break;
		}
	}
	
	if (!phys) {
		return false;
	}
	
	phys->flags = flags;
	
	return true;
}

bool entity_phys_set_mass(World *world, uint32_t id, float mass) {
	CPhysics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == id) {
			phys = (world->CPhysicss + i);
			break;
		}
	}
	
	if (!phys) {
		return false;
	}
	
	phys->mass = mass;
	
	return true;
}

bool entity_phys_add_force(World *world, uint32_t id, DgVec3 pos, DgVec3 rot) {
	CPhysics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == id) {
			phys = (world->CPhysicss + i);
			break;
		}
	}
	
	if (!phys) {
		return false;
	}
	
	phys->Fpos = DgVec3Scale(phys->mass, pos);
	phys->Frot = DgVec3Scale(phys->mass, rot);
	
	return true;
}
