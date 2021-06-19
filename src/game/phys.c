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
	
	if (world_get_pause(world)) {
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
				
				C_Transform *ot = &world->trans[i];
				
				DgVec3 bHigh = DgVec3Add(ot->pos, ot->scale);
				DgVec3 bLow = DgVec3Subtract(ot->pos, ot->scale);
				
				// Determine if they are colliding
				bool res = (bHigh.x >= aLow.x) && (aHigh.x >= bLow.x)
					&& (bHigh.y >= aLow.y) && (aHigh.y >= bLow.y)
					&& (bHigh.z >= aLow.z) && (aHigh.z >= bLow.z);
				
				if (res) {
					// Find out how much each penerates eachother
					DgVec3 d = DgVec3Subtract(bHigh, aLow);
					char smallest = 0;
					
					// Determine which axe has the smallest penetration
					if      (d.x < d.y && d.x < d.z) { smallest = 1; }
					else if (d.y < d.x && d.y < d.z) { smallest = 2; }
					else if (d.z < d.x && d.z < d.y) { smallest = 3; }
					
					// Do it.
					switch (smallest) {
						case 1: trans->pos.x += d.x; break;
						case 2: trans->pos.y += d.y; phys->flags = phys->flags | QR_PHYS_GROUNDED; break;
						case 3: trans->pos.z += d.z; break;
					}
					
					if (!((phys->flags & QR_PHYS_DISABLE_GRAVITY) == QR_PHYS_DISABLE_GRAVITY)) {
						phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, -GRAVITY_FORCE * phys->mass, 0.0f));
					}
				}
			}
		}
	}
}
