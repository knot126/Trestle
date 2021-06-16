/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics System
 */

#include <inttypes.h>
#include <stdio.h>

#include "../util/maths.h"
#include "../world/compo.h"
#include "../world/world.h"

#include "phys.h"

const float GRAVITY_FORCE = -9.81f * 1.1f;

void phys_update(World *world, float delta) {
	/**
	 * Update Game Physics
	 * ===================
	 * 
	 * This function will update game physics for a given world on all entities
	 * that have physics.
	 */
	
	if (world->paused) {
		return;
	}
	
	for (uint32_t i = 0; i < world->phys_count; i++) {
		uint32_t id = world->phys[i].base.id;
		C_Physics *phys = &world->phys[i];
		C_Transform *trans = NULL;
		
		// Find the transfrom component
		for (uint32_t i = 0; i < world->trans_count; i++) {
			if (world->trans[i].base.id == id) {
				trans = &world->trans[i];
				break;
			}
		}
		
		if (!trans) {
			continue;
		}
		
		if (!((phys->flags & QR_PHYS_DISABLE_GRAVITY) == QR_PHYS_DISABLE_GRAVITY)) {
			phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, GRAVITY_FORCE * phys->mass, 0.0f));
		}
		
		phys->Vpos = DgVec3Add(phys->Vpos, DgVec3Scale((1.0f / phys->mass) * delta, phys->Fpos));
		phys->Vrot = DgVec3Add(phys->Vrot, DgVec3Scale((1.0f / phys->mass) * delta, phys->Frot));
		
		trans->pos = DgVec3Add(trans->pos, DgVec3Scale(delta, phys->Vpos));
		trans->rot = DgVec3Add(trans->rot, DgVec3Scale(delta, phys->Vrot));
		
		phys->Fpos = DgVec3New(0.0f, 0.0f, 0.0f);
		phys->Frot = DgVec3New(0.0f, 0.0f, 0.0f);
		
		if ((phys->flags & QR_PHYS_ENABLE_RESPONSE) == QR_PHYS_ENABLE_RESPONSE) {
			phys->flags = (phys->flags & (~QR_PHYS_GROUNDED));
			C_Transform *shape = trans;
			
			DgVec3 aHigh = DgVec3Add(shape->pos, shape->scale);
			DgVec3 aLow = DgVec3Subtract(shape->pos, shape->scale);
			
			for (uint32_t i = 0; i < world->trans_count; i++) {
				if (shape->base.id == world->trans[i].base.id) {
					continue;
				}
				
				DgVec3 bHigh = DgVec3Add(world->trans[i].pos, world->trans[i].scale);
				DgVec3 bLow = DgVec3Subtract(world->trans[i].pos, world->trans[i].scale);
				
				bool res = (bHigh.x >= aLow.x) && (aHigh.x >= bLow.x)
					&& (bHigh.y >= aLow.y) && (aHigh.y >= bLow.y)
					&& (bHigh.z >= aLow.z) && (aHigh.z >= bLow.z);
				
				if (res) {
					phys->flags = phys->flags | QR_PHYS_GROUNDED;
					
					DgVec3 force_out = DgVec3Subtract(world->trans[i].pos, shape->pos);
					trans->pos.y += bHigh.y - aLow.y;
					
					if (!((phys->flags & QR_PHYS_DISABLE_GRAVITY) == QR_PHYS_DISABLE_GRAVITY)) {
						phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, -GRAVITY_FORCE * phys->mass, 0.0f));
					}
				}
			}
		}
	}
}
