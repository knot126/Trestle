/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the vulkan graphics core.
 */

#pragma once

#include <vulkan/vulkan.h>

// #include "../math/vector2.h"
// #include "../math/vector3.h"
#include "../graphics/vulkan.h"

DgVulkanInfo* graphics_init(void);
void graphics_free(DgVulkanInfo*);

DgOpenGLContext* gl_graphics_init(void);
void gl_graphics_free(DgOpenGLContext*);
