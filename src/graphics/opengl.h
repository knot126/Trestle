/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */
#pragma once

#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "../world/world.h"

typedef unsigned int GLuint;

typedef struct DgOpenGLContext {
	GLFWwindow* window;
	GLuint* programs;
	size_t programs_count;
	GLuint* shaders;
	size_t shader_count;
	GLuint* textures;
	size_t textures_count;
} DgOpenGLContext;

DgOpenGLContext* gl_graphics_init(void);
void gl_graphics_update(World *world, DgOpenGLContext* gl);
void gl_graphics_free(DgOpenGLContext* gl);
void gl_handle_input(DgOpenGLContext* gl);
bool gl_get_should_keep_open(DgOpenGLContext *info);
