/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Systems Manager
 */

#include <string.h>
#include <stdio.h>

#include "graphics/graphics.h"

#include "systems.h"

void sys_init(SystemStates *sys) {
	// Set to null
	memset(sys, 0, sizeof(SystemStates));
	
	// Graphics initialisation
	printf("Init graphics subsystem...\n");
	sys->graphics = graphics_init(DG_GRAPHICS_TYPE_OPENGL);
}

void sys_destroy(SystemStates *sys) {
	printf("Destroying graphics subsystem...\n");
	graphics_free(sys->graphics);
}
