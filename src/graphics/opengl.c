/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#include <string.h>
#include <math.h>

#if !defined(DG_GLEW_INCLUDED_OK)
	#include "../glew/glew.h"
	#define DG_GLEW_INCLUDED_OK
#endif
#include <GLFW/glfw3.h>

#include "../util/alloc.h"
#include "../util/time.h"
#include "../io/load.h"

#include "opengl.h"

GLuint gl_load_shader(char* filename, GLenum type) {
	/* 
	 * Load and create an OpenGL shader `filename`, of type `type`.
	 */
	char* path = DgEvalPath(filename);
	
	DgLoadBinaryFileInfo* shader_source = DgLoadBinaryFile(path);
	
	if (!shader_source) {
		printf("Failed to load shader '%s'.\n", filename);
		return 0;
	}
	
	// Create shader and load source
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, 
		(const GLchar* const *) &shader_source->data, 
		(const GLint *) &shader_source->size);
	
	// Cleanup path
	DgUnloadBinaryFile(shader_source);
	DgFree(path);
	
	// Compile the shader
	glCompileShader(shader);
	
	// Check that the shader was compiled successfully
	GLint status;
	char mesg[512];
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shader, 512, NULL, mesg);
	
	if (mesg[0]) {
		printf("%s\n", mesg);
	}
	
	if (status != GL_TRUE) {
		return 0;
	}
	
	printf("Loaded shader at '%s'.\n", filename);
	
	return shader;
}

GLenum gl_error_check(char* file, int line) {
	GLenum e = glGetError();
	
	if (e) {
		printf("At %s:%d : ", file, line);
	}
	
	switch (e) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("OpenGL: Invalid enum: a bad enum was passed to a function.\n");
			break;
		case GL_INVALID_VALUE:
			printf("OpenGL: Invalid value: a bad value was passed to a function.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("OpenGL: Invalid operation: the operation is not allowed in the current state.\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("OpenGL: Framebuffer not ready: the framebuffer was not ready for the operation.\n");
			break;
		case GL_OUT_OF_MEMORY:
			printf("OpenGL: Out of memory: the system has run out of memory.\n");
			break;
	}
}

GLuint gl_make_program(uint32_t shader_count, GLuint* shaders) {
	// Create program, add shaders to it and link it
	GLuint program_id = glCreateProgram();
	
	for (int i = 0; i < shader_count; i++) {
		glAttachShader(program_id, shaders[i]);
	}
	
	glLinkProgram(program_id);
	
	// Check for any errors making program
	GLint link_stat;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_stat);
	
	if (!link_stat) {
		char mesg[1024];
		glGetProgramInfoLog(program_id, 1024, NULL, mesg);
		if (mesg[0]) {
			printf("%s\n", mesg);
		}
	}
	
	return program_id;
}

void gl_set_window_size(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

DgOpenGLContext* gl_graphics_init(void) {
	DgOpenGLContext* gl = DgAlloc(sizeof(DgOpenGLContext));
	memset(gl, 0, sizeof(DgOpenGLContext));
	
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
	gl->window = glfwCreateWindow(1280, 720, "Decent Games Engine", NULL, NULL);
	
	if (!gl->window) {
		printf("Failed to create glfw window.\n");
		exit(-1);
	}
	
	glfwMakeContextCurrent(gl->window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(gl->window, gl_set_window_size);
	glViewport(0, 0, 1280, 720);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Load shaders
	gl->shaders = DgAlloc(sizeof(GLuint *) * 3);
	
	GLuint current_shader = gl_load_shader("assets://shaders/vertex.glsl", GL_VERTEX_SHADER);
	if (!current_shader) {
		exit(EXIT_FAILURE);
	}
	gl->shaders[0] = current_shader;
	
	current_shader = gl_load_shader("assets://shaders/frag.glsl", GL_FRAGMENT_SHADER);
	if (!current_shader) {
		exit(EXIT_FAILURE);
	}
	gl->shaders[1] = current_shader;
	
	current_shader = gl_load_shader("assets://shaders/frag2.glsl", GL_FRAGMENT_SHADER);
	if (!current_shader) {
		exit(EXIT_FAILURE);
	}
	gl->shaders[2] = current_shader;
	
	gl->shader_count = 3;
	
	gl->programs = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	
	GLuint my_shaders[2];
	my_shaders[0] = gl->shaders[0];
	my_shaders[1] = gl->shaders[1];
	gl->programs[0] = gl_make_program(2, my_shaders);
	
	GLuint my_shaders_[2];
	my_shaders_[0] = gl->shaders[0];
	my_shaders_[1] = gl->shaders[2];
	gl->programs[1] = gl_make_program(2, my_shaders_);
	
	// Delete shaders, they are not needed anymore
	for (int i = 0; i < gl->shader_count; i++) {
		glDeleteShader(gl->shaders[i]);
	}
	
	// Vertex datas
	const float data1[] = {
		 -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
		 -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 
	};
	
	const float data2[] = {
		  0.5f,  0.5f,  0.0f,
		 -0.5f,  0.5f,  0.0f,
		  0.5f, -0.5f,  0.0f,
	};
	
	const float *datas[] = {(float *) &data1, (float *) &data2};
	
	gl->vaos = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	gl->vaos_count = 2;
	gl->vbos = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	gl->vbos_count = 2;
	
	// Create a VAOs
	glGenVertexArrays(gl->vaos_count, gl->vaos);
	
	// Making a VBOs
	glGenBuffers(gl->vbos_count, gl->vbos);
	
	// For the first VAO
	glBindVertexArray(gl->vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data1), datas[0], GL_STATIC_DRAW);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Tell OpenGL about this vertex data
	GLint attr_Position = glGetAttribLocation(gl->programs[0], "position");
	GLint attr_Colour = glGetAttribLocation(gl->programs[0], "colour");
	
	glVertexAttribPointer(attr_Position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(attr_Position);
	
	glVertexAttribPointer(attr_Colour, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
	glEnableVertexAttribArray(attr_Colour);
	
	gl_error_check(__FILE__, __LINE__);
	
	// For the second VAO
	glBindVertexArray(gl->vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data2), datas[1], GL_STATIC_DRAW);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Tell OpenGL about this vertex data
	attr_Position = glGetAttribLocation(gl->programs[1], "position");
	
	glVertexAttribPointer(attr_Position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attr_Position);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Offset testing
	glUseProgram(gl->programs[0]);
	glUniform3f(glGetUniformLocation(gl->programs[0], "distort"), 0.1f, 0.3f, 0.4f);
	
	glUseProgram(0);
	
	printf("Graphics subsystem has been initialised.\n");
	
	return gl;
}

void gl_graphics_free(DgOpenGLContext* gl) {
	glfwTerminate();
	
	for (int i = 0; i < gl->programs_count; i++) {
		glDeleteProgram(gl->programs[i]);
	}
	
	for (int i = 0; i < gl->vbos_count; i++) {
		glDeleteBuffers(gl->vbos_count, gl->vbos);
	}
	
	for (int i = 0; i < gl->vaos_count; i++) {
		glDeleteVertexArrays(gl->vaos_count, gl->vaos);
	}
	
	DgFree(gl->shaders);
	DgFree(gl);
}

void gl_graphics_update(DgOpenGLContext* gl) {
	// Normal OpenGL events
	
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
	
	// OpenGL clear and draw
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(gl->programs[0]);
	glBindVertexArray(gl->vaos[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glUseProgram(gl->programs[1]);
	
	float time, green;
	green = (sin(DgTime()) / 2.0f) + 0.5f;
	
	int loc = glGetUniformLocation(gl->programs[1], "solid");
	
	glUniform3f(loc, 0.0f, green, 0.0f);
	glBindVertexArray(gl->vaos[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebos[0]);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	
	if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gl->window, GL_TRUE);
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_Q) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

bool gl_get_should_keep_open(DgOpenGLContext *info) {
	return !glfwWindowShouldClose(info->window);
}
