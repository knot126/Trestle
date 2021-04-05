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

bool entity_set_transform(World *world, uint32_t id, DgVec3 pos, DgVec3 rot) {
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
	
	return true;
}
