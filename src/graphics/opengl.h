/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */
#pragma once

#if !defined(DG_GLEW_INCLUDED_OK)
	#include "../../lib/glew/glew.h"
	#define DG_GLEW_INCLUDED_OK
#endif

#include <stdbool.h>
#include <GLFW/glfw3.h>

typedef struct DgOpenGLContext {
	GLFWwindow* window;
	GLuint* vaos;
	size_t vaos_count;
	GLuint* vbos;
	size_t vbos_count;
	GLuint* ebos;
	size_t ebos_count;
	GLuint* programs;
	size_t programs_count;
	GLuint* shaders;
	size_t shader_count;
	GLuint* textures;
	size_t textures_count;
	
	size_t element_count; // number of elements in loaded data
} DgOpenGLContext;

GLuint gl_load_shader(char* filename, GLenum type);
GLenum gl_error_check(char* file, int line);

DgOpenGLContext* gl_graphics_init(void);
void gl_graphics_update(DgOpenGLContext* gl);
void gl_graphics_free(DgOpenGLContext* gl);
void gl_handle_input(DgOpenGLContext* gl);
bool gl_get_should_keep_open(DgOpenGLContext *info);
