/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 */

#include <stdbool.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

#include "../graphics/opengl.h"

#include "glfwi.h"

char DG_glfwi_presses[GLFW_KEY_LAST];

static void glfwi_callback_keypress(GLFWwindow *context, int key, int scancode, int action, int flags) {
	DG_glfwi_presses[key] = action;
	//printf("Get key %d, action %d.\n", key, action);
}

bool glfwi_get_key(int key, int mode) {
	return (!!DG_glfwi_presses[key]);
}

void glfwi_init(DgOpenGLContext *context) {
	glfwSetKeyCallback(context->window, &glfwi_callback_keypress);
}
