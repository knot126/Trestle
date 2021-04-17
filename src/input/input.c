/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System
 */

#include <stdbool.h>

#include "../graphics/opengl.h"
#include "../graphics/graphics.h"
#include "glfwi.h"

void input_init(GraphicsInitInfo info) {
	glfwi_init((DgOpenGLContext *) info.info);
}

void input_update(GraphicsInitInfo info) {
	gl_handle_input((DgOpenGLContext *) info.info);
}

bool getKeyPressed(int key) {
	glfwi_get_key(key, GLFW_PRESS);
}
