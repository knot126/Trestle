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

#include "../generic/world.h"
#include "../util/alloc.h"
#include "../util/time.h"
#include "../util/fail.h"
#include "../util/maths.h"
#include "../util/rand.h"
#include "../io/load.h"
#include "../types.h" // For g_deltaTime
#include "image.h"

#include "opengl.h"

DgVec3 campos;

DgVec2 mouse_delta;
DgVec2 mouse_last;

const float camSpeed = 2.5f;
const float camSenseitivity = 0.01f;
float pitch = 0.0f, yaw = -0.125f;
DgVec3 camfwd;

// Yes, it's odd, but I really rather not include the header files in this file.
#include "glutils.h"

void gl_set_window_size(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void gl_update_mouse(GLFWwindow* window, double x, double y) {
	/*
	 * Handles the mouse movement input
	 */
	static bool first = false;
	
	if (first) {
		mouse_last.x = x;
		mouse_last.y = y;
		first = true;
	}
	
	mouse_delta.x = x - mouse_last.x;
	mouse_delta.y = y - mouse_last.y;
	
	mouse_last = DgVec2New(x, y);
}

DgOpenGLContext* gl_graphics_init(void) {
	/*
	 * Initialise any global OpenGL graphics state. In the future, this should 
	 * not be in chrage of things like the camera, but this is how it is for the
	 * moment as I am still trying to play around with everything.
	 * 
	 * Returns a (DgOpenGLContext *) to created info structure.
	 */
	
	// Create context info structure
	DgOpenGLContext* gl = DgAlloc(sizeof(DgOpenGLContext));
	memset(gl, 0, sizeof(DgOpenGLContext));
	
	// Glfw init
	glfwInit();
	
	// Window paramaters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
	// Create window
	gl->window = glfwCreateWindow(1280, 720, "Trestle Engine", NULL, NULL);
	
	if (!gl->window) {
		DgFail("Failed to create glfw window.", -1);
	}
	
	glfwMakeContextCurrent(gl->window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(gl->window, gl_set_window_size);
	glfwSetCursorPosCallback(gl->window, gl_update_mouse);
	glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	// TODO: Move to a mesh entity
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
	
	// Make random colours
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
	
	// Write a file with the cube mesh
	DgFileStream *s = DgFileStreamOpen("./cube.bin", "wb");
	
	uint32_t temp;
	temp = sizeof(data1) / 32;
	DgFileStreamWriteInt32(s, &temp);
	DgFileStreamWrite(s, sizeof(data1), data1);
	temp = sizeof(indicies) / 4;
	DgFileStreamWriteInt32(s, &temp);
	DgFileStreamWrite(s, sizeof(indicies), indicies);
	
	DgFileStreamClose(s);
	
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
	
	// Loading the data that VAO will need
	s = DgFileStreamOpen("./cube.bin", "rb");
	if (!s) {
		DgFail("Failed to open file stream to read data.\n", -1);
	}
	
	uint32_t vbo_size;
	DgFileStreamReadInt32(s, &vbo_size);
	float * vbo_data = DgAlloc(vbo_size * 32);
	if (!vbo_data) {
		DgFail("Failed to load VAO data from file.\n", -1);
	}
	DgFileStreamRead(s, vbo_size * 32, vbo_data);
	
	uint32_t ebo_size;
	DgFileStreamReadInt32(s, &ebo_size);
	uint32_t * ebo_data = DgAlloc(ebo_size * 4);
	if (!ebo_data) {
		DgFail("Failed to load EBO data from file.\n", -1);
	}
	DgFileStreamRead(s, ebo_size * 4, ebo_data);
	
	DgFileStreamClose(s);
	
	// Make sure first VAO is the active one
	glBindVertexArray(gl->vaos[0]);
	
	// VBO setting data
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * 32, vbo_data, GL_STATIC_DRAW);
	gl_error_check(__FILE__, __LINE__);
	
	// Index buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size * sizeof(float), ebo_data, GL_STATIC_DRAW);
	
	// Free the filey things from earlier
	DgFree(ebo_data);
	DgFree(vbo_data);
	
	// Check for errors
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
	
	mouse_delta = DgVec2New(720.0f / 2.0f, 1280.0f / 2.0f);
	campos = DgVec3New(1.0f, 0.0f, 3.0f);
	
	gl_error_check(__FILE__, __LINE__);
	
	printf("Graphics subsystem has been initialised.\n");
	
	return gl;
}

void gl_graphics_update(DgOpenGLContext* gl) {
	/*
	 * Update OpenGL-related state and the graphics system
	 */
	
	// Normal OpenGL events
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
	
	// OpenGL clear and draw
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(gl->programs[0]);
	
	// Calculate the projection matrix
	// We use the current window width and height to calculate the aspect ratio
	// for each frame, allowing dynamic resize of the window.
	int w, h;
	glfwGetWindowSize(gl->window, &w, &h);
	DgMat4 proj = DgMat4NewPerspective2(0.125f, (float) w / (float) h, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "proj"), 1, GL_TRUE, &proj.ax);
	
	// Calculate view matrix (camera transform)
	// TODO: I think either the perspective view is still messing things up or
	// I have somehow done this wrong. See the this is broken near the pitch in
	// input processing function to get an idea of what is going on.
	camfwd = DgVec3Normalise(DgVec3New(
		DgCos(yaw) * DgCos(pitch), 
		DgSin(pitch), 
		DgSin(yaw) * DgCos(pitch)));
	DgMat4 camera = DgTransformLookAt(campos, DgVec3Add(campos, camfwd), DgVec3New(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	
	// Bind the currently active textures for this shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl->textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->textures[1]);
	
	glBindVertexArray(gl->vaos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl->ebos[0]);
	
	for (int i = -4; i < 4; i++) {
		for (int j = -3; j < 3; j++) {
			for (int k = -1; k < 2; k++) {
				DgMat4 model = DgMat4Translate(DgMat4New(1.0f), DgVec3New(((float)i)*2.0f, ((float)j)*2.0f, ((float)k)*2.0f));
				glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "model"), 1, GL_TRUE, &model.ax);
				glDrawElements(GL_TRIANGLES, gl->element_count, GL_UNSIGNED_INT, 0);
			}
		}
	}
	/*
	 * This will need to be done in the future for each entity in the world:
	
	for (size_t i = 0; i < world->count; i++) {
		if (world->masks[i] & (QR_COMPONENT_MESH) == (QR_COMPONENT_MESH)) {
			// do stuff here
		}
	}
	
	*/
	
	// Check for errors
	gl_error_check(__FILE__, __LINE__);
}

void gl_handle_input(DgOpenGLContext* gl) {
	/*
	 * Handles most game input
	 */
	if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(gl->window, GL_TRUE);
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_UP) == GLFW_PRESS) {
		campos = DgVec3Add(campos, DgVec3Scale(camSpeed * g_deltaTime, camfwd));
	}
	if (glfwGetKey(gl->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		campos = DgVec3Subtract(campos, DgVec3Scale(camSpeed * g_deltaTime, camfwd));
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		campos = DgVec3Add(campos, DgVec3Scale(camSpeed * g_deltaTime, DgVec3Cross(camfwd, DgVec3New(0.0f, 1.0f, 0.0f))));
	}
	if (glfwGetKey(gl->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		campos = DgVec3Subtract(campos, DgVec3Scale(camSpeed * g_deltaTime, DgVec3Cross(camfwd, DgVec3New(0.0f, 1.0f, 0.0f))));
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
	
	yaw += mouse_delta.x * camSenseitivity * g_deltaTime;
	
	// TODO: This is broken ! 
	//pitch += mouse_delta.y * camSenseitivity * g_deltaTime;
	
	if (pitch > 0.249f) {
		pitch = 0.249f;
	}
	
	if (pitch < -0.249f) {
		pitch = -0.249f;
	}
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

bool gl_get_should_keep_open(DgOpenGLContext *info) {
	return !glfwWindowShouldClose(info->window);
}
