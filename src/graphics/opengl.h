/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */
#pragma once

//#include "../glew/glew.h"

typedef struct DgOpenGLContext {
	GLFWwindow* window;
} DgOpenGLContext;

GLuint gl_load_shader(char* filename, GLenum type);
