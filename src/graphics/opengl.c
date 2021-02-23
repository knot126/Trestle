/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#include <string.h>
#include <math.h>

#if !defined(DG_GLEW_INCLUDED_OK)
	#include "../../lib/glew/glew.h"
	#define DG_GLEW_INCLUDED_OK
#endif
#include <GLFW/glfw3.h>

#include "../util/alloc.h"
#include "../util/time.h"
#include "../util/fail.h"
#include "../io/load.h"
#include "image.h"

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

static void gl_load_texture(DgOpenGLContext *gl, char *path) {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->textures[1]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	DgImageInfo image = DgLoadImage(path);
	
	if (!image.data) {
		DgFail("Failed to load texture.\n", -1);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	DgFreeImage(&image);
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
	
	gl->shader_count = 3;
	
	gl->programs = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	
	GLuint my_shaders[2];
	my_shaders[0] = gl->shaders[0];
	my_shaders[1] = gl->shaders[1];
	gl->programs[0] = gl_make_program(2, my_shaders);
	
	// Delete shaders, they are not needed anymore
	for (int i = 0; i < gl->shader_count; i++) {
		glDeleteShader(gl->shaders[i]);
	}
	
	// Vertex datas
	const float data1[] = {
		// X      Y     Z     U     V     R     G     B
		 -0.8f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
		 -0.8f, -0.8f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		  0.8f, -0.8f, 0.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 
		  0.8f,  0.8f, 0.0f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
	};
	
	const int indicies[] = {
		0, 1, 2,
		0, 2, 3,
	};
	
	gl->vaos = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	gl->vaos_count = 2;
	
	if (!gl->vaos) {
		DgFail("VAO Alloc error\n", -1);
	}
	
	gl->vbos = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	gl->vbos_count = 2;
	
	if (!gl->vaos) {
		DgFail("VBO Alloc error\n", -1);
	}
	
	gl->ebos = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	gl->ebos_count = 2;
	
	if (!gl->vaos) {
		DgFail("EBO Alloc error\n", -1);
	}
	
	// Create a VAOs
	glGenVertexArrays(gl->vaos_count, gl->vaos);
	
	// Making a VBOs
	glGenBuffers(gl->vbos_count, gl->vbos);
	
	// Same for EBOs
	glGenBuffers(gl->ebos_count, gl->ebos);
	
	// VAO Binding
	glBindVertexArray(gl->vaos[0]); // Make sure first VAO is the active one
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data1), data1, GL_STATIC_DRAW);
	
	// Index buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Tell OpenGL about this vertex data
	GLint attr_Position = glGetAttribLocation(gl->programs[0], "position");
	GLint attr_Texture = glGetAttribLocation(gl->programs[0], "texturepos");
	GLint attr_Colour = glGetAttribLocation(gl->programs[0], "colour");
	
	if (attr_Position < 0) {
		printf("No attribute Position.\n");
	}
	
	if (attr_Texture < 0) {
		printf("No attribute Texture.\n");
	}
	
	if (attr_Colour < 0) {
		printf("No attribute Colour.\n");
	}
	
	glVertexAttribPointer(attr_Position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(attr_Position);
	
	gl_error_check(__FILE__, __LINE__);
	
	glVertexAttribPointer(attr_Texture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(attr_Texture);
	
	gl_error_check(__FILE__, __LINE__);
	
	glVertexAttribPointer(attr_Colour, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
	glEnableVertexAttribArray(attr_Colour);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Making a texture
	gl->textures_count = 2;
	gl->textures = (GLuint *) DgAlloc(sizeof(GLuint) * gl->textures_count);
	
	if (!gl->textures) {
		DgFail("Texture list allocation failure.\n", -1);
	}
	
	// Making textures
	gl_load_texture(gl, "assets://gfx/1.jpg");
	gl_load_texture(gl, "assets://gfx/2.jpg");
	
	glUseProgram(gl->programs[0]);
	glUniform1i(glGetUniformLocation(gl->programs[0], "image"), 0);
	glUniform1i(glGetUniformLocation(gl->programs[0], "image2"), 1);
	
	gl_error_check(__FILE__, __LINE__);
	
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
	
	DgFree(gl->textures);
	DgFree(gl->shaders);
	DgFree(gl->vaos);
	DgFree(gl->vbos);
	DgFree(gl->ebos);
	DgFree(gl);
}

void gl_graphics_update(DgOpenGLContext* gl) {
	// Normal OpenGL events
	
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
	
	// OpenGL clear and draw
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(gl->programs[0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl->textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->textures[1]);
	
	glBindVertexArray(gl->vaos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebos[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	gl_error_check(__FILE__, __LINE__);
	
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
