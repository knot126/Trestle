/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Window management system
 */ 

#pragma once

#include "misc/grpreq.h"
#include "util/maths.h"
#include "window/framebuffer.h"

/**
 * Window
 * ======
 * 
 * A window provides both graphics and input services from the operating system.
 * It can be used to check inputs and draw out graphics.
 */
typedef struct Window_t {
	GLFWwindow *window;
	Framebuffer framebuffer;
} Window_t;

typedef int Keycode;

Window_t *window_init(Window_t *this);
void window_free(Window_t *this);

void window_set_title(Window_t *this, const char * title);
DgVec2I window_get_size(Window_t *this);
void window_set_size(Window_t *this, DgVec2I size);

void window_update(Window_t *this);

bool window_get_key(Window_t *this, Keycode key);
bool window_get_mouse_button(Window_t *this, Keycode key);
DgVec2 window_get_cursor_position(Window_t *this);

Framebuffer *window_get_framebuffer(Window_t *this);
