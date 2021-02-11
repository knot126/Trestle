/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the vulkan graphics core.
 */

#pragma once

typedef enum GraphicsType {
	DG_GRAPHICS_TYPE_OPENGL = 1,
	DG_GRAPHICS_TYPE_VULKAN = 2,
} GraphicsType;

typedef struct GraphicsInitInfo {
	void* info;
	GraphicsType type;
} GraphicsInitInfo;

GraphicsInitInfo graphics_init(GraphicsType type);
void graphics_update(GraphicsInitInfo info);
bool get_should_keep_open(GraphicsInitInfo info);
void graphics_free(GraphicsInitInfo info);
