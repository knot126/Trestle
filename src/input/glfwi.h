/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 */

#pragma once

#include <stdbool.h>

#include <GLFW/glfw3.h>

#include "util/maths.h"

bool glfwi_get_key(int key, int mode);
DgVec2 glfwi_get_mouse_pos(void);
DgVec2 glfwi_get_mouse_delta(void);
void glfwi_init(void *context);
