/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game-specific and Gameplay functions (these should be moved to lua 
 * eventually)
 */

#include <GLFW/glfw3.h>

#include "../input/input.h"
#include "../util/maths.h"
#include "../world/world.h"
#include "../world/compo.h"

void gameplay_update(World *world) {
	CPhysics *phys = NULL;
	
	for (size_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == 3) {
			phys = &world->CPhysicss[i];
			break;
		}
	}
	
	if (!phys) {
		return;
	}
	
	if (getKeyPressed(GLFW_KEY_UP)) {
		phys->Vpos = DgVec3Add(phys->Vpos, DgVec3New(0.0f, 0.0f, -0.03f));
	}
	
	else if (getKeyPressed(GLFW_KEY_DOWN)) {
		phys->Vpos = DgVec3Add(phys->Vpos, DgVec3New(0.0f, 0.0f, 0.03f));
	}
	
	else if (getKeyPressed(GLFW_KEY_LEFT)) {
		phys->Vpos = DgVec3Add(phys->Vpos, DgVec3New(-0.03f, 0.0f, 0.0f));
	}
	
	else if (getKeyPressed(GLFW_KEY_RIGHT)) {
		phys->Vpos = DgVec3Add(phys->Vpos, DgVec3New(0.03f, 0.0f, 0.0f));
	}
	
	else {
		phys->Vpos = DgVec3New(0.0f, 0.0f, 0.0f);
	}
}
