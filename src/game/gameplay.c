/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game-specific and Gameplay functions (these should be moved to lua 
 * eventually)
 */

#include <stdio.h>

#include <GLFW/glfw3.h>

#include "../input/input.h"
#include "../util/maths.h"
#include "../world/world.h"
#include "../world/compo.h"
#include "../types.h"

void gameplay_update(World *world) {
	/*
	 * Gameplay/game specific realted updates
	 */
	
	// PLAYER
	
	CPhysics *phys = NULL;
	
	for (size_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == world->player_info.id) {
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
	
	// CAMERA
	
	CTransform *ppos = NULL;
	
	for (size_t i = 0; i < world->CTransforms_count; i++) {
		if (world->CTransforms[i].base.id == world->player_info.id) {
			ppos = &world->CTransforms[i];
			break;
		}
	}
	
	if (ppos) {
		static float last;
		last += g_deltaTime;
		if (last > 1.0f) {
			printf("Player pos: (%.3f, %.3f, %.3f)\n", ppos->pos.x, ppos->pos.y, ppos->pos.z);
			last = 0.0f;
		}
	}
	else {
		printf("player not found\n");
	}
	
	CTransform *cpos = NULL;
	
	for (size_t i = 0; i < world->CTransforms_count; i++) {
		if (world->CTransforms[i].base.id == world->CCameras_active[2]) {
			cpos = &world->CTransforms[i];
			break;
		}
	}
	
	if (!ppos || !cpos) {
		return;
	}
	
	cpos->pos = DgVec3New(ppos->pos.x, ppos->pos.y + 3.0f, ppos->pos.z + 3.0f);
}
