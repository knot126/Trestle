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
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		uint32_t id = world->CPhysicss[i].base.id;
		CPhysics phys = world->CPhysicss[i];
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
		
		trans->pos = DgVec3Add(trans->pos, DgVec3Scale(delta, phys.Vpos));
		trans->rot = DgVec3Add(trans->rot, DgVec3Scale(delta, phys.Vrot));
	}
}
