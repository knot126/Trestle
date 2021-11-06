/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Window management system
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "misc/grpreq.h"
#include "util/maths.h"

#include "window.h"

size_t window_count; // Only used for tracking GLFW initialised state

Window *window_init(Window *this) {
	/**
	 * Initialise and create a window.
	 */
	
	memset(this, 0, sizeof *this);
	
	if (window_count++ == 0) {
		if (!glfwInit()) {
			return NULL;
		}
		else {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}
	}
	
	this->window = glfwCreateWindow(1280, 720, "Set window title using window_set_title(window, title)", NULL, NULL);
	
	if (!this->window) {
		return NULL;
	}
	
	return this;
}

void window_destroy(Window *this) {
	/**
	 * Destroy a window
	 */
	
	glfwDestroyWindow(this->window);
	
	if (--window_count == 0) {
		glfwTerminate();
	}
}

void window_set_title(Window *this, const char * title) {
	/**
	 * Set the title of the window to get given title.
	 */
	
	glfwSetWindowTitle(this->window, title);
}

DgVec2I window_get_size(Window *this) {
	/**
	 * Get the size of a window.
	 */
	
	DgVec2I s;
	
	glfwGetWindowSize(this->window, &s.y, &s.x);
	
	return s;
}

void window_set_size(Window *this, DgVec2I size) {
	/**
	 * Set the size of the window.
	 */
	
	glfwSetWindowSize(this->window, size.y, size.x);
}

bool window_get_key(Window *this, Keycode key) {
	/**
	 * Get the event that has happened to the keycode
	 */
	
	return (glfwGetKey(this->window, key) == GLFW_PRESS);
}

bool window_get_mouse_button(Window *this, Keycode key) {
	/**
	 * Get the state of the given mouse button.
	 */
	
	return (glfwGetMouseButton(this->window, key) == GLFW_PRESS);
}

DgVec2 window_get_cursor_position(Window *this) {
	/**
	 * Get the position of the cursor.
	 */
	
	double x, y;
	glfwGetCursorPos(this->window, &x, &y);
	return (DgVec2) {x, y};
}
