/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 */

#pragma once

#include <stdbool.h>

#include <GLFW/glfw3.h>

#include "../graphics/opengl.h"

bool glfwi_get_key(int key, int mode);
void glfwi_init(DgOpenGLContext *context);