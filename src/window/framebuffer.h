/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Window management system
 */ 

#pragma once

#include "misc/grpreq.h"
#include "util/maths.h"

/**
 * Framebuffer
 * ===========
 * 
 * Provides an abstract framebuffer. This shouldn't really be used too often for
 * drawing, but it's fine to use it say once a frame. Other things should really
 * happen on the GPU (probably using Vulkan).
 */
typedef struct Framebuffer {
	DgVec4 *data;
	uint16_t x, y;
} Framebuffer;
