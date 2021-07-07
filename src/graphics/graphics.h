/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics System Wrapper - this is still here for legacy reasons
 */

#pragma once

#include <stdbool.h>

#include "world/world.h"
#include "graphics/opengl.h"

GraphicsSystem *graphics_init(void);
void graphics_update(GraphicsSystem *this, World *world);
bool get_should_keep_open(GraphicsSystem *this);
void graphics_free(GraphicsSystem *this);
