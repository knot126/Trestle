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

#include "../world/world.h"
#include "../util/alloc.h"
#include "../util/time.h"
#include "../util/fail.h"
#include "../util/maths.h"
#include "../util/rand.h"
#include "../util/load.h"
#include "../types.h" // For g_deltaTime
#include "image.h"

#include "opengl.h"

DgVec3 campos;
DgVec3 camrot;

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
	
	// Allocate memory for the various types of data
	gl->vaos = (GLuint *) DgAlloc(sizeof(GLuint) * 2);
	gl->vaos_count = 2;
	
	if (!gl->vaos) {
		DgFail("VAO Alloc error\n", -1);
	}
	
	// Create a VAOs
	glGenVertexArrays(gl->vaos_count, gl->vaos);
	
	// Make sure first VAO is the active one
	glBindVertexArray(gl->vaos[0]);
	
	// Check for errors
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
	campos = DgVec3New(0.0f, 0.0f, 3.0f);
	
	gl_error_check(__FILE__, __LINE__);
	
	printf("Graphics subsystem has been initialised.\n");
	
	return gl;
}

void gl_graphics_update(World *world, DgOpenGLContext *gl) {
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
	camfwd = DgVec3New(
		DgCos(yaw) * DgCos(pitch),
		DgSin(pitch),
		DgSin(yaw) * DgCos(pitch)
	);
	DgMat4 camera = DgTransformLookAt2(DgVec3New(0.0f, 1.0f, 3.0f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(0.0f, 1.0f, 0.0f));
	//DgMat4Print(camera);
	//DgMat4 camera = DgMat4Translate(DgMat4New(1.0f), DgVec3New(0.0f, 0.0f, -3.0f));
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	
	// Bind the currently active textures for this shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl->textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->textures[1]);
	
	glBindVertexArray(gl->vaos[0]);
	
	for (size_t i = 0; i < world->CMeshs_count; i++) {
		uint32_t id = world->CMeshs[i].base.id;
		//printf("Info: Rendering entity %d with mesh node.\n", id);
		
		// Push new verticies if needed
		if (world->CMeshs[i].updated) {
			if (!world->CMeshs[i].vbo) {
				glGenBuffers(1, &world->CMeshs[i].vbo);
			}
			
			if (!world->CMeshs[i].ebo) {
				glGenBuffers(1, &world->CMeshs[i].ebo);
			}
			
			glBindBuffer(GL_ARRAY_BUFFER, world->CMeshs[i].vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->CMeshs[i].ebo);
			
			glBufferData(GL_ARRAY_BUFFER, world->CMeshs[i].vert_count * 32, world->CMeshs[i].vert, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, world->CMeshs[i].index_count * sizeof(uint32_t), world->CMeshs[i].index, GL_STATIC_DRAW);
			
			gl_error_check(__FILE__, __LINE__);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, world->CMeshs[i].vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->CMeshs[i].ebo);
		
		gl_set_format(gl);
		
		// Find the transform
		DgVec3 translate = DgVec3New(0.0f, 0.0f, 0.0f);
		
		for (int i = 0; i < world->CTransforms_count; i++) {
			if (world->CTransforms[i].base.id == id) {
				translate = world->CTransforms[i].pos;
				break;
			}
		}
		
		DgMat4 model = DgMat4Translate(DgMat4New(1.0f), translate);
		glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "model"), 1, GL_TRUE, &model.ax);
		
		glDrawElements(GL_TRIANGLES, world->CMeshs[i].index_count, GL_UNSIGNED_INT, 0);
		
		gl_error_check(__FILE__, __LINE__);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
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
	pitch += mouse_delta.y * camSenseitivity * g_deltaTime;
	
	if (pitch > 0.249f) {
		pitch = 0.249f;
	}
	
	if (pitch < -0.249f) {
		pitch = -0.249f;
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_T) == GLFW_PRESS) {
		if (camrot.x == 0.075f) {
			camrot.x = 0.0f;
		}
		else {
			camrot.x = 0.075f;
		}
	}
}

void gl_graphics_free(DgOpenGLContext* gl) {
	glfwTerminate();
	
	for (int i = 0; i < gl->programs_count; i++) {
		glDeleteProgram(gl->programs[i]);
	}
	
	DgFree(gl->textures);
	DgFree(gl->shaders);
	DgFree(gl->vaos);
	DgFree(gl);
}

bool gl_get_should_keep_open(DgOpenGLContext *info) {
	return !glfwWindowShouldClose(info->window);
}
