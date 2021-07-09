/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game-specific and Gameplay functions (these should be moved to lua 
 * eventually)
 */

#pragma once

#include "world/world.h"
#include "graphics/opengl.h"

void gameplay_update(World *world, GraphicsSystem *graphics);
