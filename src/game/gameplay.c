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

float speed = 2.0f;

void gameplay_update(World *world) {
	/*
	 * Gameplay/game specific realted updates
	 */
	
	// PLAYER
	
	CTransform *transf = NULL;
	CPhysics *phys = NULL;
	
	for (size_t i = 0; i < world->CTransforms_count; i++) {
		if (world->CTransforms[i].base.id == world->player_info.id) {
			transf = &world->CTransforms[i];
			break;
		}
	}
	
	for (size_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == world->player_info.id) {
			phys = &world->CPhysicss[i];
			break;
		}
	}
	
	if (!transf || !phys) {
		return;
	}
	
	float lspeed = speed * g_deltaTime;
	
// 	if (getKeyPressed(GLFW_KEY_UP)) {
// 		transf->pos = DgVec3Add(transf->pos, DgVec3New(0.0f, 0.0f, -lspeed));
// 	}
// 	
// 	if (getKeyPressed(GLFW_KEY_DOWN)) {
// 		transf->pos = DgVec3Add(transf->pos, DgVec3New(0.0f, 0.0f, lspeed));
// 	}
	
	if (getKeyPressed(GLFW_KEY_UP)) {
		speed = speed + g_deltaTime;
	}
	
	if (getKeyPressed(GLFW_KEY_DOWN)) {
		speed = speed - g_deltaTime;
	}
	
	transf->pos = DgVec3Add(transf->pos, DgVec3New(0.0f, 0.0f, -lspeed));
	
	if (getKeyPressed(GLFW_KEY_LEFT)) {
		transf->pos = DgVec3Add(transf->pos, DgVec3New(-lspeed, 0.0f, 0.0f));
	}
	
	if (getKeyPressed(GLFW_KEY_RIGHT)) {
		transf->pos = DgVec3Add(transf->pos, DgVec3New(lspeed, 0.0f, 0.0f));
	}
	
	if (getKeyPressed(GLFW_KEY_SPACE)) {
		phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, speed * speed, 0.0f));
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
	
	cpos->pos = DgVec3New(ppos->pos.x * 0.2f, ppos->pos.y + 4.0f, ppos->pos.z + 4.0f);
}
