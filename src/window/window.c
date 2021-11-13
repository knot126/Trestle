/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Window management system
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#define DG_INCLUDE_NATIVE_WINDOW_API
#include "misc/grpreq.h"
#undef DG_INCLUDE_NATIVE_WINDOW_API
#include "util/maths.h"
#include "window/framebuffer.h"

#include "window.h"

size_t window_count; // Only used for tracking GLFW initialised state

Window_t *window_init(Window_t *this) {
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

void window_free(Window_t *this) {
	/**
	 * Destroy a window
	 */
	
	glfwDestroyWindow(this->window);
	
	if (--window_count == 0) {
		glfwTerminate();
	}
}

void window_set_title(Window_t *this, const char * title) {
	/**
	 * Set the title of the window to get given title.
	 */
	
	glfwSetWindowTitle(this->window, title);
}

DgVec2I window_get_size(Window_t *this) {
	/**
	 * Get the size of a window.
	 */
	
	DgVec2I s;
	
	glfwGetWindowSize(this->window, &s.y, &s.x);
	
	return s;
}

void window_set_size(Window_t *this, DgVec2I size) {
	/**
	 * Set the size of the window.
	 */
	
	glfwSetWindowSize(this->window, size.y, size.x);
}

static void window_update_fb(Window_t *this, Framebuffer *framebuffer) {
	/**
	 * Update the window contents using a specific framebuffer.
	 */
	
#if !defined(QR_WINDOW_DISABLE_XORG)
	// Get X display, window and size
	Display *display = glfwGetX11Display();
	Window window = glfwGetX11Window(this->window);
	DgVec2I size = window_get_size(this);
	
	// Create graphics context (needed to draw)
	XGCValues gc_values;
	GC context = XCreateGC(display, window, 0, &gc_values);
	
	// Flush event queue
	XSync(display, False);
	
	// Draw points
	for (int y = 0; /*(y < framebuffer->y) &&*/ (y < size.y); y++) {
		for (int x = 0; /*(x < framebuffer->x) &&*/ (x < size.x); x++) {
			XDrawPoint(display, window, context, y, x);
		}
	}
	
	// Flush event queue
	XSync(display, False);
#endif
}

void window_update(Window_t *this) {
	/**
	 * Update the contents of a window.
	 */
	
	window_update_fb(this, &this->framebuffer);
}

bool window_get_key(Window_t *this, Keycode key) {
	/**
	 * Get the event that has happened to the keycode
	 */
	
	return (glfwGetKey(this->window, key) == GLFW_PRESS);
}

bool window_get_mouse_button(Window_t *this, Keycode key) {
	/**
	 * Get the state of the given mouse button.
	 */
	
	return (glfwGetMouseButton(this->window, key) == GLFW_PRESS);
}

DgVec2 window_get_cursor_position(Window_t *this) {
	/**
	 * Get the position of the cursor.
	 */
	
	double x, y;
	glfwGetCursorPos(this->window, &x, &y);
	return (DgVec2) {x, y};
}

Framebuffer *window_get_framebuffer(Window_t *this) {
	/**
	 * Get a direct pointer to the framebuffer.
	 */
	
	return &this->framebuffer;
}

void window_set_framebuffer(Window_t *this, Framebuffer *framebuffer) {
	/**
	 * Set the window's current framebuffer. The framebuffer given can be
	 * freed after this since the framebuffer's memory will be copied.
	 */
	
	// TODO: Free old framebufer (there are not functions for that yet)
	
	this->framebuffer = *framebuffer;
}

/**
 * For drawing in Xorg directly:
 * 
 *  - Pixmap XCreatePixmap(Display *display, Drawable d, unsigned int width, unsigned int height, unsigned int depth);
 *  - Draw to the pixmap with the contents
 *     - XDrawPoint(Display *display, Drawable d, GC gc, int x, int y);
 *     - XDrawPoints(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
 *  - XSetWindowBackgroundPixmap(Display *display, Window w, Pixmap background_pixmap); -- cannot be used any longer
 *  - int XFreePixmap(Display *display, Pixmap pixmap);
 *  - repeat for each frame
 */
