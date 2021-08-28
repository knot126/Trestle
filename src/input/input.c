/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System
 */

#include <stdbool.h>

#include "graphics/opengl.h"
#include "graphics/graphics.h"
#include "input/glfwi.h"

#include "input.h"

void input_init(InputSystem *input, GraphicsSystem *graphics) {
	/**
	 * Initialise the input system. This depends on the graphics subsystem when
	 * using the GLFW interface.
	 */
	
	input->graphics = graphics;
	
	glfwi_init(input->graphics);
}

void input_update(InputSystem *input) {
	/**
	 * Update the input keypresses.
	 */
	
	gl_handle_input(input->graphics);
}

bool getKeyPressed(int key) {
	/**
	 * Return true if a key is considered pressed or held down, false otherwise.
	 */
	
	glfwi_get_key(key, GLFW_PRESS);
}

DgVec2 getMousePos(void) {
	/**
	 * Return the current position of the mouse pointer, down to the subpixel if
	 * it is supported.
	 */
	
	return glfwi_get_mouse_pos();
}

DgVec2 getMouseDelta(void) {
	/**
	 * Return the difference between the last mouse position and the current 
	 * one, down to the subpixel if supported.
	 */
	
	return glfwi_get_mouse_delta();
}
