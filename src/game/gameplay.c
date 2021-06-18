/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game-specific and Gameplay functions (these should be moved to lua 
 * eventually)
 */

#include <stdio.h>

#include "graphics/gl_incl.h"

#include "../input/input.h"
#include "../util/maths.h"
#include "../util/log.h"
#include "../util/ini.h"
#include "../world/world.h"
#include "../world/compo.h"
#include "../types.h"

#include "gameplay.h"

void gameplay_update(World *world) {
	/*
	 * Gameplay/game specific realted updates
	 */
	
	static bool initial = true;
	if (initial) {
		world->player_info.speed = world->game.speed_min;
		initial = false;
	}
	
	float speed = world->player_info.speed;
	float speed_min, speed_max;
	
	world_get_speed(world, &speed_min, &speed_max);
	
	if (world_get_pause(world)) {
		return;
	}
	
	// PLAYER
	C_Transform *transf = entity_find_trans(world, world->player_info.id);
	C_Physics *phys = entity_find_phys(world, world->player_info.id);
	
	if (!transf || !phys) {
		return;
	}
	
	// Increse or decrase speed
	if (getKeyPressed(GLFW_KEY_UP)) {
		speed = speed + (g_deltaTime * 2.0f);
	}
	
	if (getKeyPressed(GLFW_KEY_DOWN)) {
		speed = speed - (g_deltaTime * 2.0f);
	}
	
	// Check speed range
	if (speed < speed_min) {
// 		printf("speed < speed_min\n");
		speed = speed_min;
	}
	
	if (speed > speed_max) {
// 		printf("speed > speed_max\n");
		speed = speed_max;
	}
	
	// Set speed if changed
	world->player_info.speed = speed;
// 	printf("Speed: %f, %f\n", world->player_info.speed, speed);
	
	// Move player forward
	transf->pos = DgVec3Add(transf->pos, DgVec3New(0.0f, 0.0f, -speed * g_deltaTime));
	
	// Move player side to side
	if (getKeyPressed(GLFW_KEY_LEFT)) {
		transf->pos = DgVec3Add(transf->pos, DgVec3New(-2.5f * g_deltaTime, 0.0f, 0.0f));
	}
	
	if (getKeyPressed(GLFW_KEY_RIGHT)) {
		transf->pos = DgVec3Add(transf->pos, DgVec3New(2.5f * g_deltaTime, 0.0f, 0.0f));
	}
	
	// Jumping control
	static int8_t jump_control_frame;
	
	if (getKeyPressed(GLFW_KEY_SPACE) && ((phys->flags & QR_PHYS_GROUNDED) == QR_PHYS_GROUNDED) && jump_control_frame == 0) {
		phys->Fpos = DgVec3Add(phys->Fpos, DgVec3New(0.0f, 1500.0f, 0.0f));
		jump_control_frame = 15;
	}
	
	if (jump_control_frame > 0) {
		jump_control_frame--;
	}
	
	// Print player position log every second
	{
		static float last;
		last += g_deltaTime;
		if (last > 1.0f) {
			DgLog(DG_LOG_VERBOSE, "Player pos: (%.3f, %.3f, %.3f)", transf->pos.x, transf->pos.y, transf->pos.z);
			last = 0.0f;
		}
	}
	
	C_Transform *cpos = entity_find_trans(world, world->cam_active[2]);
	
	if (!cpos) {
		return;
	}
	
	cpos->pos = DgVec3New(transf->pos.x * 0.2f, transf->pos.y + 4.0f, transf->pos.z + 4.0f);
}
