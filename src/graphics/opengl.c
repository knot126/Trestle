/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#include <string.h>

#if !defined(DG_GLEW_INCLUDED_OK)
	#include "../glew/glew.h"
	#define DG_GLEW_INCLUDED_OK
#endif
#include <GLFW/glfw3.h>

#include "../util/alloc.h"
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
	
	// Vertex datas
	const float g_Triangle[] = {
		 -0.5f,  0.5f,  0.0f, // 0
		 -0.5f, -0.5f,  0.0f, // 1
		  0.5f, -0.5f,  0.0f, // 2
		
		  0.5f,  0.5f,  0.0f, // 3
	};
	
	const int g_Triangle_elements[] = {
		0, 1, 2,
		1, 2, 3,
	};
	
	gl->vert_count = sizeof(g_Triangle) / 3;
	
	// Making a VBO
	glGenBuffers(1, &gl->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Triangle), g_Triangle, GL_STATIC_DRAW);
	
	// Making an EBO for the VBO
	glGenBuffers(1, &gl->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_Triangle_elements), g_Triangle_elements, GL_STATIC_DRAW);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Load shaders
	gl->shaders = DgAlloc(sizeof(GLuint *) * 2);
	
	GLuint vertex_shader = gl_load_shader("assets://shaders/vertex.glsl", GL_VERTEX_SHADER);
	if (!vertex_shader) {
		exit(EXIT_FAILURE);
	}
	gl->shaders[0] = vertex_shader;
	
	GLuint fragment_shader = gl_load_shader("assets://shaders/frag.glsl", GL_FRAGMENT_SHADER);
	if (!fragment_shader) {
		exit(EXIT_FAILURE);
	}
	gl->shaders[1] = fragment_shader;
	
	gl->shader_count = 2;
	
	// Create program and add shaders to it
	gl->program = glCreateProgram();
	glAttachShader(gl->program, vertex_shader);
	glAttachShader(gl->program, fragment_shader);
	glBindFragDataLocation(gl->program, 0, "out_colour");
	glLinkProgram(gl->program);
	
	GLint link_stat;
	glGetProgramiv(gl->program, GL_LINK_STATUS, &link_stat);
	
	if (!link_stat) {
		char mesg[512];
		glGetProgramInfoLog(gl->program, 512, NULL, mesg);
		printf("%s\n", mesg);
	}
	
	glUseProgram(gl->program);
	
	// Delete shaders, they are not needed anymore
	for (int i = 0; i < gl->shader_count; i++) {
		glDeleteShader(gl->shaders[i]);
	}
	
	gl_error_check(__FILE__, __LINE__);
	
	// Create a VAO
	glGenVertexArrays(1, &gl->vao);
	glBindVertexArray(gl->vao);
	
	// Tell OpenGL about this vertex data
	GLint attr_Position = glGetAttribLocation(gl->program, "position");
	
	glVertexAttribPointer(attr_Position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attr_Position);
	
	gl_error_check(__FILE__, __LINE__);
	
	return gl;
}

void gl_graphics_free(DgOpenGLContext* gl) {
	glfwTerminate();
	
	glDeleteProgram(gl->program);
	
	glDeleteBuffers(1, &gl->vbo);
	glDeleteVertexArrays(1, &gl->vao);
	
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
	
	//glBindVertexArray(gl->vao);
	//glDrawArrays(GL_TRIANGLES, 0, gl->vert_count);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gl->window, GL_TRUE);
	}
}

bool gl_get_should_keep_open(DgOpenGLContext *info) {
	return !glfwWindowShouldClose(info->window);
}
