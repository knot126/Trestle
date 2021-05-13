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

void gameplay_update(World * const restrict world) {
	/*
	 * Gameplay/game specific realted updates
	 */
	
	// PLAYER
	
	if (world->player.id < 0) {
		return;
	}
	
	C_Transform *transf = &world->trans[world->ent.trans[world->player.id - 1]];
	C_Physics *phys = &world->phys[world->ent.phys[world->player.id - 1]];
	
	float lspeed = speed * g_deltaTime;
	
	if (getKeyPressed(GLFW_KEY_UP)) {
		speed = speed + (g_deltaTime * 3.0f);
	}
	
	if (getKeyPressed(GLFW_KEY_DOWN)) {
		speed = speed - (g_deltaTime * 3.0f);
	}
	
	transf->pos = DgVec3Add(transf->pos, DgVec3New(0.0f, 0.0f, -lspeed));
	
	if (getKeyPressed(GLFW_KEY_LEFT)) {
		transf->pos = DgVec3Add(transf->pos, DgVec3New(-lspeed, 0.0f, 0.0f));
	}
	
	if (getKeyPressed(GLFW_KEY_RIGHT)) {
		transf->pos = DgVec3Add(transf->pos, DgVec3New(lspeed, 0.0f, 0.0f));
	}
	
	if (getKeyPressed(GLFW_KEY_SPACE)) {
		phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, speed * 6.0f, 0.0f));
	}
	
	// CAMERA
	
	C_Transform *ppos = transf;
	
	static float last;
	last += g_deltaTime;
	if (last > 1.0f) {
		printf("Player pos: (%.3f, %.3f, %.3f)\n", ppos->pos.x, ppos->pos.y, ppos->pos.z);
		last = 0.0f;
	}
	
	C_Transform *cpos = &world->trans[world->ent.trans[world->camera_active - 1]];
	
	cpos->pos = DgVec3New(ppos->pos.x * 0.2f, ppos->pos.y + 4.0f, ppos->pos.z + 4.0f);
}
