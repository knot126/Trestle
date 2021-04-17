/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the vulkan graphics core.
 */

#pragma once

#include <stdbool.h>

#include "../world/world.h"

typedef enum GraphicsType {
	DG_GRAPHICS_TYPE_OPENGL = 1,
	DG_GRAPHICS_TYPE_VULKAN = 2,
} GraphicsType;

typedef struct GraphicsInitInfo {
	void* info;
	GraphicsType type;
} GraphicsInitInfo;

typedef struct Args_Threaded_graphics_update {
	World *world;
	GraphicsInitInfo info;
} Args_Threaded_graphics_update;

GraphicsInitInfo graphics_init(void);
void graphics_update(World *world, GraphicsInitInfo info);
bool get_should_keep_open(GraphicsInitInfo info);
void graphics_free(GraphicsInitInfo info);

void *Threaded_graphics_update(void *data);
