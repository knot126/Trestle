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
#include "../util/maths.h"
#include "../util/rand.h"
#include "../io/load.h"
#include "image.h"

#include "opengl.h"

DgVec3 campos;

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
	
	// Construct the shader source string from some strings and a file
	GLchar *strings[3];
	GLint strings_lengths[3];
	
	strings[0] = "#version 420 core\n";
	switch (type) {
		case GL_VERTEX_SHADER:
			strings[1] = "#define VERTEX\n\n";
			break;
		case GL_FRAGMENT_SHADER:
			strings[1] = "#define FRAGMENT\n\n";
			break;
		case GL_GEOMETRY_SHADER:
			strings[1] = "#define GEOMETRY\n\n";
			break;
	}
	strings[2] = (GLchar *) shader_source->data;
	
	strings_lengths[0] = strlen(strings[0]);
	strings_lengths[1] = strlen(strings[1]);
	strings_lengths[2] = shader_source->size;
	
	// Create shader and load source
	GLuint shader = glCreateShader(type);
	glShaderSource(shader,
		3, 
		(const GLchar* const *) strings, 
		(const GLint *) strings_lengths);
	
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
	
	//printf("Loaded shader from '%s'.\n", filename);
	
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

GLuint gl_make_program(DgOpenGLContext *gl, char* source_path) {
	// Create program, add shaders to it and link it
	GLuint vertex = gl_load_shader(source_path, GL_VERTEX_SHADER);
	if (!vertex) {
		return 0;
	}
	
	GLuint fragment = gl_load_shader(source_path, GL_FRAGMENT_SHADER);
	if (!fragment) {
		return 0;
	}
	
	gl->shader_count += 2;
	
	// Allocate memory to store shader IDs
	if (!gl->shaders) {
		gl->shaders = DgAlloc(sizeof(GLuint) * gl->shader_count);
	}
	else {
		gl->shaders = DgRealloc(gl->shaders, sizeof(GLuint) * gl->shader_count);
	}
	
	if (!gl->shaders) {
		DgFail("Error: Failed to allocate memory for shaders list. Abort!!", -4);
		return 0;
	}
	else {
		gl->shaders[gl->shader_count - 2] = vertex;
		gl->shaders[gl->shader_count - 1] = fragment;
	}
	
	GLuint program_id = glCreateProgram();
	
	glAttachShader(program_id, vertex);
	glAttachShader(program_id, fragment);
	
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
		return 0;
	}
	
	glUseProgram(program_id);
	
	return program_id;
}

void gl_set_window_size(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

static void gl_load_texture(DgOpenGLContext *gl, char *path, GLenum active_texture) {
	static int texture_count = 0;
	
	glActiveTexture(active_texture);
	glBindTexture(GL_TEXTURE_2D, gl->textures[texture_count]);
	texture_count++;
	
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
	
	printf("Loaded image of id %d.\n", texture_count);
	
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
	
	gl->window = glfwCreateWindow(1280, 720, "Trestle Engine", NULL, NULL);
	
	if (!gl->window) {
		DgFail("Failed to create glfw window.", -1);
	}
	
	glfwMakeContextCurrent(gl->window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(gl->window, gl_set_window_size);
	glViewport(0, 0, 1280, 720);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Load shaders
	gl->programs = (GLuint *) DgAlloc(sizeof(GLuint) * 1);
	gl->programs[0] = gl_make_program(gl, "assets://shaders/main.glsl");
	
	// Delete shaders, they are not needed anymore
	for (int i = 0; i < gl->shader_count; i++) {
		glDeleteShader(gl->shaders[i]);
	}
	
	// Vertex datas
	//DgLoadBinaryFileInfo vertexes = DgLoadBinaryFile();
	
	float data1[] = {
		// X      Y      Z     U     V     R     G     B
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		 
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		 
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		 
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f,
	};
	
	for (int i = 0; i < 24; i++) {
		data1[(i * 8) + 5] = DgRandFloat();
		data1[(i * 8) + 6] = DgRandFloat();
		data1[(i * 8) + 7] = DgRandFloat();
	}
	
	const int indicies[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};
	
	gl->element_count = sizeof(indicies) / sizeof(int);
	
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
	
	glGenTextures(gl->textures_count, gl->textures);
	
	// Making textures
	gl_load_texture(gl, "assets://gfx/1.jpg", GL_TEXTURE0);
	gl_load_texture(gl, "assets://gfx/2.jpg", GL_TEXTURE1);
	
	glUniform1i(glGetUniformLocation(gl->programs[0], "image"), 0);
	glUniform1i(glGetUniformLocation(gl->programs[0], "image2"), 1);
	glUseProgram(0);
	
	glEnable(GL_DEPTH_TEST);
	
	campos = DgVec3New(1.0f, 0.0f, 3.0f);
	
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

static void gl_handle_input(DgOpenGLContext* gl) {
	if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gl->window, GL_TRUE);
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_UP) == GLFW_PRESS) {
		campos.z -= 0.01f;
	}
	if (glfwGetKey(gl->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		campos.z += 0.01f;
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		campos.x += 0.01f;
	}
	if (glfwGetKey(gl->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		campos.x -= 0.01f;
	}
	
	static bool polymode = false;
	if (glfwGetKey(gl->window, GLFW_KEY_Q) == GLFW_PRESS) {
		polymode = !polymode;
	}
	
	if (polymode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void gl_graphics_update(DgOpenGLContext* gl) {
	// Normal OpenGL events
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
	
	// OpenGL clear and draw
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(gl->programs[0]);
	
	// trying to get things looking better at all resolutions
	int w, h;
	glfwGetWindowSize(gl->window, &w, &h);
	
	campos = DgVec3New(DgSin(DgTime() * 0.1f) * 5.0f, 0.0f, DgCos(DgTime() * 0.1f) * 5.0f);
	DgVec3 lookpoint = DgVec3New(0.0f, 0.0f, 0.0f);
	
	//DgVec3New(1.0f * DgSin(DgTime() * 0.2f), 1.0f * DgCos(DgTime() * 0.2f), 1.0f * DgSin(DgTime() * 0.2f))
	
	DgMat4 camera = DgTransformLookAt(campos, lookpoint, DgVec3New(0.0f, 1.0f, 0.0f));
	DgMat4 proj = DgMat4NewPerspective2(0.125f, (float) w / (float) h, 0.1f, 100.0f);
	
	
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "proj"), 1, GL_TRUE, &proj.ax);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl->textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->textures[1]);
	
	glBindVertexArray(gl->vaos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebos[0]);
	
	for (int i = -4; i < 4; i++) {
		for (int j = -3; j < 3; j++) {
			DgMat4 model = DgMat4Translate(DgMat4New(1.0f), DgVec3New(((float)i)*2.0f, ((float)j)*2.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "model"), 1, GL_TRUE, &model.ax);
			glDrawElements(GL_TRIANGLES, gl->element_count, GL_UNSIGNED_INT, 0);
		}
	}
	
	
	gl_error_check(__FILE__, __LINE__);
	
	gl_handle_input(gl);
}

bool gl_get_should_keep_open(DgOpenGLContext *info) {
	return !glfwWindowShouldClose(info->window);
}
