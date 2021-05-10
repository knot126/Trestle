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
		
		if ((phys->flags & QR_PHYS_ENABLE_RESPONSE) == QR_PHYS_ENABLE_RESPONSE) {
			CTransform *shape = trans;
			
			DgVec3 aHigh = DgVec3Add(shape->pos, shape->scale);
			DgVec3 aLow = DgVec3Subtract(shape->pos, shape->scale);
			
			for (uint32_t i = 0; i < world->CTransforms_count; i++) {
				if (shape->base.id == world->CTransforms[i].base.id) {
					continue;
				}
				
				DgVec3 bHigh = DgVec3Add(world->CTransforms[i].pos, world->CTransforms[i].scale);
				DgVec3 bLow = DgVec3Subtract(world->CTransforms[i].pos, world->CTransforms[i].scale);
				
				bool res = (bHigh.x >= aLow.x) && (aHigh.x >= bLow.x)
					&& (bHigh.y >= aLow.y) && (aHigh.y >= bLow.y)
					&& (bHigh.z >= aLow.z) && (aHigh.z >= bLow.z);
				
				if (res) {
					printf("Collision!!\n");
					
					DgVec3 force_out = DgVec3Subtract(world->CTransforms[i].pos, shape->pos);
					printf("Difference: (%f, %f, %f)\n", force_out.x, force_out.y, force_out.z);
					shape->pos.y = world->CTransforms[i].scale.y - force_out.y;
				}
			}
		}
	}
}
