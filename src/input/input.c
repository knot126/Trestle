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
