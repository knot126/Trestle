/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "../generic/world.h"

void world_init(World *world, size_t prealloc_count) {
	memset(world, 0, sizeof(World));
	
	if (prealloc_count == 0) {
		return;
	}
	
	
}
