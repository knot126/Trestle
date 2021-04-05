/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System
 */

#include "../graphics/opengl.h"
#include "../graphics/graphics.h"

void input_update(GraphicsInitInfo info) {
	gl_handle_input((DgOpenGLContext *) info.info);
}
