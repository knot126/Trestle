/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 */

#pragma once

#include <stdbool.h>

#include "graphics/gl_incl.h"
#include "../graphics/opengl.h"

bool glfwi_get_key(int key, int mode);
DgVec2 glfwi_get_mouse_pos(void);
DgVec2 glfwi_get_mouse_delta(void);
void glfwi_init(GraphicsSystem *context);
