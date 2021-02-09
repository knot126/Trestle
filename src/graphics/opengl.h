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
	GLuint vao;
	GLuint vbo;
	GLuint program;
	GLuint* shaders;
	size_t shader_count;
} DgOpenGLContext;

GLuint gl_load_shader(char* filename, GLenum type);
GLenum gl_error_check(char* file, int line);
