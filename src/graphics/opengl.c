/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>

#if !defined(DG_GLEW_INCLUDED_OK)
	#include "glad.h"
	#define DG_GLEW_INCLUDED_OK 1
#endif
#include <GLFW/glfw3.h>

#include "../world/world.h"
#include "../util/alloc.h"
#include "../util/time.h"
#include "../util/fail.h"
#include "../util/maths.h"
#include "../util/rand.h"
#include "../util/load.h"
#include "../util/ini.h"
#include "../util/str.h"
#include "../util/bitmap.h"
#include "../util/log.h"
#include "../types.h" // For g_deltaTime
#include "image.h"

#include "opengl.h"

// Yes, it's odd, but I really rather not include the header files in this file.
#include "glutils.h"

void gl_set_window_size(GLFWwindow* window, int w, int h) {
	glViewport(16, 16, w, h);
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
	int w_width = atol(DgINIGet(g_quickRunConfig, "Main", "window_width", "1280"));
	int w_height = atol(DgINIGet(g_quickRunConfig, "Main", "window_height", "720"));
	
	// Create window
	char *w_title = DgStrcadf(DgStrcad("Main", " ― Quick Run "), DgINIGet(g_quickRunConfig, "Distribution", "version", "[unknown version]"));
	gl->window = glfwCreateWindow(w_width, w_height, w_title, NULL, NULL);
	DgFree(w_title);
	
	if (!gl->window) {
		DgLog(DG_LOG_FATAL, "Failed to create glfw window");
		return NULL;
	}
	
	glfwMakeContextCurrent(gl->window);
	
	int ret = gladLoadGLLoader( (GLADloadproc) &glfwGetProcAddress );
	if (!ret) {
		return NULL;
		DgLog(DG_LOG_FATAL, "Failed to load GLAD");
	}
	
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(gl->window, gl_set_window_size);
	glViewport(0, 0, w_width, w_height);
	
	// set window icon
	DgImageInfo icon = DgLoadImage("assets://icon.png");
	if (icon.data) {
		GLFWimage icons[1];
		icons[0].pixels = (unsigned char *) icon.data;
		icons[0].width = icon.width;
		icons[0].height = icon.height;
		glfwSetWindowIcon(gl->window, sizeof(icons) / sizeof(GLFWimage), icons);
		
		DgFreeImage(&icon);
	}
	else {
		printf("\033[1;33mWarning:\033[0m Could not set window icon.\n");
	}
	
	gl_error_check(__FILE__, __LINE__);
	
	/*
	float data1[] = {
		// X      Y      Z     U     V     R     G     B
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		-1.0f, -1.0f,  1.0f, 2.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		 1.0f, -1.0f,  1.0f, 2.0f, 2.0f, 0.85f, 0.85f, 0.85f,
		 1.0f,  1.0f,  1.0f, 0.0f, 2.0f, 0.85f, 0.85f, 0.85f,
		 
		-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		-1.0f, -1.0f, -1.0f, 2.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		 1.0f, -1.0f, -1.0f, 2.0f, 2.0f, 0.85f, 0.85f, 0.85f,
		 1.0f,  1.0f, -1.0f, 0.0f, 2.0f, 0.85f, 0.85f, 0.85f,
		
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		-1.0f,  1.0f, -1.0f, 2.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		 1.0f,  1.0f, -1.0f, 2.0f, 2.0f, 0.95f, 0.95f, 0.95f,
		 1.0f,  1.0f,  1.0f, 0.0f, 2.0f, 0.95f, 0.95f, 0.95f,
		 
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		-1.0f, -1.0f, -1.0f, 2.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		 1.0f, -1.0f, -1.0f, 2.0f, 2.0f, 0.95f, 0.95f, 0.95f,
		 1.0f, -1.0f,  1.0f, 0.0f, 2.0f, 0.95f, 0.95f, 0.95f,
		 
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		 1.0f, -1.0f, -1.0f, 2.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		 1.0f,  1.0f, -1.0f, 2.0f, 2.0f, 0.75f, 0.75f, 0.75f,
		 1.0f,  1.0f,  1.0f, 0.0f, 2.0f, 0.75f, 0.75f, 0.75f,
		
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		-1.0f, -1.0f, -1.0f, 2.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		-1.0f,  1.0f, -1.0f, 2.0f, 2.0f, 0.75f, 0.75f, 0.75f,
		-1.0f,  1.0f,  1.0f, 0.0f, 2.0f, 0.75f, 0.75f, 0.75f,
	};
	
	// Make random colours
// 	for (int i = 0; i < 24; i++) {
// 		data1[(i * 8) + 5] = DgRandFloat();
// 		data1[(i * 8) + 6] = DgRandFloat();
// 		data1[(i * 8) + 7] = DgRandFloat();
// 	}
	
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
	*/
	
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
		DgLog(DG_LOG_FATAL, "Could not allocate memory for vertex array objects");
		return NULL;
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
		DgLog(DG_LOG_FATAL, "Texture list allocation failure");
		return NULL;
	}
	
	glGenTextures(gl->textures_count, gl->textures);
	
	// Making textures
	DgBitmap *bmp = DgBitmapGenTiles(256, 256, 128);
	if (bmp) {
		gl_load_texture_buffer(gl, bmp, GL_TEXTURE0);
		DgBitmapFree(bmp);
	}
	
	glUniform1i(glGetUniformLocation(gl->programs[0], "image"), 0);
	glUseProgram(0);
	
	// Alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	gl_error_check(__FILE__, __LINE__);
	
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
	DgMat4 camera;
	
	// Do the camera
	if (world->CCameras_active[0] != 0) {
		uint32_t tid = world->CCameras_active[0] - 1, cid = world->CCameras_active[1] - 1;
// 		printf("Camera pos = (%f, %f, %f)\n", 
// 			   world->CTransforms[tid].pos.x,
// 			   world->CTransforms[tid].pos.y,
// 			   world->CTransforms[tid].pos.z);
		camera = DgTransfromBasicCamera(world->CTransforms[tid].pos, world->CTransforms[tid].rot);
	}
	else {
		camera = DgTransformLookAt2(DgVec3New(0.0f, 1.0f, 3.0f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(0.0f, 1.0f, 0.0f));
	}
	
	// Push our matris to the GPU
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	
	// Bind the currently active textures for this shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl->textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl->textures[1]);
	
	glBindVertexArray(gl->vaos[0]);
	
	for (size_t i = 0; i < world->CMeshs_count; i++) {
		uint32_t id = world->CMeshs[i].base.id;
// 		printf("Info: Rendering entity %d with mesh node.\n", id);
		
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
		DgVec3 rotate = DgVec3New(0.0f, 0.0f, 0.0f);
		DgVec3 scale = DgVec3New(1.0f, 1.0f, 1.0f);
		
		for (int i = 0; i < world->CTransforms_count; i++) {
			if (world->CTransforms[i].base.id == id) {
				translate = world->CTransforms[i].pos;
				rotate = world->CTransforms[i].rot;
				scale = world->CTransforms[i].scale;
				break;
			}
		}
		
		DgMat4 rot_x = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(1.0f, 0.0f, 0.0f), rotate.x);
		DgMat4 rot_y = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(0.0f, 1.0f, 0.0f), rotate.y);
		DgMat4 rot_z = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(0.0f, 0.0f, 1.0f), rotate.z);
		DgMat4 rot_mat = DgMat4ByMat4Multiply(rot_z, DgMat4ByMat4Multiply(rot_y, rot_x));
		
		DgMat4 model = 
			DgMat4ByMat4Multiply(
				DgMat4Translate(DgMat4New(1.0f), translate), 
				DgMat4ByMat4Multiply(rot_mat, 
					DgMat4Scale(DgMat4New(1.0f), scale)
				)
			);
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
	if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(gl->window, GL_TRUE);
	}
	
	static bool polymode = false;
	
	if (glfwGetKey(gl->window, GLFW_KEY_Q)) {
		polymode = !polymode;
	}
	
	if (polymode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
