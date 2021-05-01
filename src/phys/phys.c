/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics System
 */

#include <inttypes.h>

#include "../util/maths.h"
#include "../world/compo.h"
#include "../world/world.h"

#include "phys.h"
 
void phys_update(World *world, float delta) {
	/**
	 * Update Game Physics
	 * ===================
	 * 
	 * This function will update game physics for a given world on all entities
	 * that have physics.
	 */
	
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		uint32_t id = world->CPhysicss[i].base.id;
		CPhysics *phys = &world->CPhysicss[i];
		CTransform *trans = NULL;
		
		// Find the transfrom component
		for (uint32_t i = 0; i < world->CTransforms_count; i++) {
			if (world->CTransforms[i].base.id == id) {
				trans = &world->CTransforms[i];
				break;
			}
		}
		
		if (!trans) {
			continue;
		}
		
		if (!((phys->flags & QR_PHYS_DISABLE_GRAVITY) == QR_PHYS_DISABLE_GRAVITY)) {
			phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, -9.81f * phys->mass, 0.0f));
		}
		
		phys->Vpos = DgVec3Add(phys->Vpos, DgVec3Scale((1.0f / phys->mass) * delta, phys->Fpos));
		phys->Vrot = DgVec3Add(phys->Vrot, DgVec3Scale((1.0f / phys->mass) * delta, phys->Frot));
		
		trans->pos = DgVec3Add(trans->pos, DgVec3Scale(delta, phys->Vpos));
		trans->rot = DgVec3Add(trans->rot, DgVec3Scale(delta, phys->Vrot));
		
		phys->Fpos = DgVec3New(0.0f, 0.0f, 0.0f);
		phys->Frot = DgVec3New(0.0f, 0.0f, 0.0f);
		
		//printf("Entity %d is at (%f, %f, %f)\n", id, trans->pos.x, trans->pos.y, trans->pos.z);
	}
}
