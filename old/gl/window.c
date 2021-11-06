/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Window
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "gl_incl.h"

#include "window.h"

int32_t window_init(Window *this) {
	/**
	 * Initialise a window.
	 */
	
	this->state = glfwCreateWindow(1280, 800, "WINDOW TITLE", NULL, NULL);
}
