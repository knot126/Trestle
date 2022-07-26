/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System (GLFW Backend)
 * 
 * @deprecated This should be replaced by something else.
 */

#include <stdbool.h>
#include <stdio.h>

#include "graphics/gl_incl.h"
#include "graphics/opengl.h"
#include "util/log.h"

#include "glfwi.h"

DgVec2 lastMousePos;
DgVec2 mousePos;
DgVec2 mouseDelta;

GraphicsSystem *input_system_context;

static void glfwi_callback_mouse(GLFWwindow *context, double x, double y) {
	/**
	 * Mouse update callback
	 */
	
	lastMousePos = mousePos;
	
	mousePos.x = x;
	mousePos.y = y;
	
	mouseDelta = DgVec2Subtract(mousePos, lastMousePos);
}

bool glfwi_get_key(int key, int mode) {
	/**
	 * Get key function
	 */
	
	int state = glfwGetKey(input_system_context->window, key);
	
	//DgLog(DG_LOG_VERBOSE, "Keypress check: [glfwGetKey(%d) = %d] == %d: %d", key, state, mode, state==mode);
	
	return state == mode;
}

DgVec2 glfwi_get_mouse_pos(void) {
	/**
	 * Mouse pos function
	 */
	
	return mousePos;
}

DgVec2 glfwi_get_mouse_delta(void) {
	/**
	 * Mouse delta function
	 */
	
	return mouseDelta;
}

void glfwi_init(GraphicsSystem *context) {
	/**
	 * Init backend
	 */
	
	glfwSetCursorPosCallback(context->window, &glfwi_callback_mouse);
	input_system_context = context;
}
