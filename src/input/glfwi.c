/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 */

#include <stdbool.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

#include "util/maths.h"

#include "glfwi.h"

char DG_glfwi_presses[GLFW_KEY_LAST];
DgVec2 lastMousePos;
DgVec2 mousePos;
DgVec2 mouseDelta;

static void glfwi_callback_keypress(GLFWwindow *context, int key, int scancode, int action, int flags) {
	DG_glfwi_presses[key] = action;
}

static void glfwi_callback_mouse(GLFWwindow *context, double x, double y) {
	lastMousePos = mousePos;
	
	mousePos.x = x;
	mousePos.y = y;
	
	mouseDelta = DgVec2Subtract(mousePos, lastMousePos);
}

bool glfwi_get_key(int key, int mode) {
	return !!DG_glfwi_presses[key];
}

DgVec2 glfwi_get_mouse_pos(void) {
	return mousePos;
}

DgVec2 glfwi_get_mouse_delta(void) {
	return mouseDelta;
}

void glfwi_init(void *context) {
	
}
