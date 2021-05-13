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

const float GRAVITY_FORCE = -9.81f;

void phys_update(World *world, float delta) {
	/**
	 * Update Game Physics
	 * ===================
	 * 
	 * This function will update game physics for a given world on all entities
	 * that have physics.
	 */
	
	for (Entity i = 0; i < world->ent_count; i++) {
		if (world->ent.phys[i] < 0 || world->ent.trans[i] < 0) {
			continue;
		}
		
		C_Physics *phys = &world->phys[world->ent.phys[i]];
		C_Transform *trans = &world->trans[world->ent.trans[i]];
		
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
			C_Transform *shape = trans;
			
			DgVec3 aHigh = DgVec3Add(shape->pos, shape->scale);
			DgVec3 aLow = DgVec3Subtract(shape->pos, shape->scale);
			
			for (Entity k = 0; k < world->ent_count; k++) {
				if (i == k || world->ent.trans[k] < 0) { continue; }
				
				DgVec3 bHigh = DgVec3Add(
					world->trans[world->ent.trans[k]].pos, 
					world->trans[world->ent.trans[k]].scale);
				DgVec3 bLow = DgVec3Subtract(
					world->trans[world->ent.trans[k]].pos, 
					world->trans[world->ent.trans[k]].scale);
				
				bool res = (bHigh.x >= aLow.x) && (aHigh.x >= bLow.x)
					&& (bHigh.y >= aLow.y) && (aHigh.y >= bLow.y)
					&& (bHigh.z >= aLow.z) && (aHigh.z >= bLow.z);
				
				if (res) {
					DgVec3 force_out = DgVec3Subtract(world->trans[world->ent.trans[k]].pos, shape->pos);
					
					trans->pos.y += bHigh.y - aLow.y;
					
					printf("%f %f \n", bHigh.x - aLow.x, bHigh.y - aLow.y);
					
					if (!((phys->flags & QR_PHYS_DISABLE_GRAVITY) == QR_PHYS_DISABLE_GRAVITY)) {
						phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, -GRAVITY_FORCE * phys->mass, 0.0f));
					}
				}
			}
		}
	}
}
