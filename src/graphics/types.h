/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics Types
 */
#pragma once

#include <inttypes.h>

typedef enum DgGraphicsType {
	DG_GRAPHICS_VULKAN = 0x02;
	DG_GRAPHICS_OPENGL = 0x03;
	DG_GRAPHICS_DIRECTX_CLASSIC = 0x12;
	DG_GRAPHICS_DIRECTX_MODERN = 0x13;
} DgGraphicsType;

typedef struct DgGraphicsInfoPointer {
	void* pointer;
	DgGraphicsType type;
} DgGraphicsInfoPointer;
