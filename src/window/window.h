/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Window management system
 */ 

#pragma once

#include "misc/grpreq.h"

/**
 * Window
 * ======
 * 
 * A window provides both graphics and input services from the operating system.
 * It can be used to check inputs and draw out graphics.
 */
typedef struct Window {
	GLFWwindow *window;
} Window;

typedef int Keycode;

Window *window_init(Window *this);
void window_destroy(Window *this);

void window_set_title(Window *this, const char * title);
DgVec2I window_get_size(Window *this);
void window_set_size(Window *this, DgVec2I size);

bool window_get_key(Window *this, Keycode key);
bool window_get_mouse_button(Window *this, Keycode key);
DgVec2 window_get_cursor_position(Window *this);
