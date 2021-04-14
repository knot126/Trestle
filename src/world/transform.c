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

bool entity_set_physics(World *world, uint32_t id, DgVec3 pos, DgVec3 rot, DgVec3 scale) {
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
	
	phys->Vpos = pos;
	phys->Vrot = rot;
	phys->Vscale = scale;
	
	return true;
}
