/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 */

#include <stdbool.h>
#include <stdio.h>

#include "graphics/gl_incl.h"
#include "graphics/opengl.h"

#include "glfwi.h"

char DG_glfwi_presses[GLFW_KEY_LAST];

static void glfwi_callback_keypress(GLFWwindow *context, int key, int scancode, int action, int flags) {
	DG_glfwi_presses[key] = action;
}

bool glfwi_get_key(int key, int mode) {
	return (!!DG_glfwi_presses[key]);
}

void glfwi_init(GraphicsSystem *context) {
	glfwSetKeyCallback(context->window, &glfwi_callback_keypress);
}
