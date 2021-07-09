/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "gl_incl.h"

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
#include "texture.h"

#include "opengl.h"

GraphicsSystem *QR_ACTIVE_GRPAHICS_SYSTEM;

GraphicsSystem *graphics(GraphicsSystem *G) {
	if (G) {
		QR_ACTIVE_GRPAHICS_SYSTEM = G;
		return G;
	}
	else {
		return QR_ACTIVE_GRPAHICS_SYSTEM;
	}
}

// Yes, it's odd, but I really rather not include the header files in this file.
#include "glutils.h"

void gl_set_window_size(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void graphics_init(GraphicsSystem * restrict gl, const World * restrict world) {
	/*
	 * Initialise any global OpenGL graphics state. In the future, this should 
	 * not be in chrage of things like the camera, but this is how it is for the
	 * moment as I am still trying to play around with everything.
	 * 
	 * Returns a (GraphicsSystem *) to created info structure.
	 */
	
	// Create context info structure
	memset(gl, 0, sizeof *gl);
	
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
		return;
	}
	
	glfwMakeContextCurrent(gl->window);
	
	int ret = gladLoadGLLoader( (GLADloadproc) &glfwGetProcAddress );
	if (!ret) {
		DgLog(DG_LOG_FATAL, "Failed to load GLAD");
		return;
	}
	
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(gl->window, &gl_set_window_size);
	
	glViewport(0, 0, w_width, w_height);
	
	// set window icon
	DgImageInfo icon = DgLoadImage(DgINIGet(g_quickRunConfig, "Window", "window_icon", "assets://icon.png"));
	if (icon.data) {
		GLFWimage icons[1];
		icons[0].pixels = (unsigned char *) icon.data;
		icons[0].width = icon.width;
		icons[0].height = icon.height;
		glfwSetWindowIcon(gl->window, sizeof(icons) / sizeof(GLFWimage), icons);
		
		DgFreeImage(&icon);
	}
	else {
		DgLog(DG_LOG_ERROR, "Could not set window icon.");
	}
	
	// Load shaders
	uint32_t res;
	
	res = graphicsLoadShader(gl, "assets://shaders/main.glsl");
	if (res) {
		// failed to load shader
		// if 2 the error affects other parts too and is fatal
	}
	
	res = graphicsLoadShader(gl, "assets://shaders/text.glsl");
	if (res) {
		// failed to load shader
		// if 2 the error affects other parts too and is fatal
	}
	
	res = graphicsLoadShader(gl, "assets://shaders/uibox.glsl");
	if (res) {
		// failed to load shader
		// if 2 the error affects other parts too and is fatal
	}
	
	glUseProgram(gl->programs[0]);
	
	// Delete shaders, they are not needed anymore
	for (int i = 0; i < gl->shader_count; i++) {
		glDeleteShader(gl->shaders[i]);
	}
	
	// Check for errors
	gl_error_check(__FILE__, __LINE__);
	
	// Create texture manager
	gltexture_init(&gl->texture);
	
	// Making textures
	DgBitmap *bmp = DgBitmapGenTiles(256, 256, 64);
	if (bmp) {
		gltexture_load_buffer(&gl->texture, "placeholder", bmp);
		gltexture_set_unit(&gl->texture, "placeholder", GL_TEXTURE0);
		DgBitmapFree(bmp);
	}
	else {
		DgLog(DG_LOG_ERROR, "Failed to generate placeholder texture.");
	}
	
	gltexture_load_list(&gl->texture, "assets://gfx/textures.xml");
	
	// Setting texture uniforms in shaders
	gl_error_check(__FILE__, __LINE__);
	
	glUseProgram(gl->programs[0]);
	glUniform1i(glGetUniformLocation(gl->programs[0], "image"), 0);
	
	glUniform1f(glGetUniformLocation(gl->programs[0], "LightFactor"), 0.15f);
	DgVec3 v = {1.0, 1.0, 0.1};
	v = DgVec3Normalise(v);
	glUniform3f(glGetUniformLocation(gl->programs[0], "LightDirection"), v.x, v.y, v.z);
	
	gl_error_check(__FILE__, __LINE__);
	
	glUseProgram(gl->programs[1]);
	glUniform1i(glGetUniformLocation(gl->programs[1], "font"), 0);
	glUniform4f(glGetUniformLocation(gl->programs[1], "colour"), 1.0, 1.0, 1.0, 1.0);
	
	gl_error_check(__FILE__, __LINE__);
	
	glUseProgram(0);
	
	// Alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Set default clear colour
	gl->clearColour = (DgVec4) {0.5f, 0.5f, 0.5f, 1.0f};
	
	// DEPRECATED BEHAVIOUR: Store world for later use
	gl->world = (World *) world;
}

void graphics_update(GraphicsSystem *gl) {
	/*
	 * Update OpenGL-related state and the graphics system
	 */
	
	//glfwMakeContextCurrent(gl->window);
	// DEPRECATED BEHAVIOUR: Reload world for use
	World *world = gl->world;
	
	// Normal OpenGL events
	glfwSwapBuffers(gl->window);
	glfwPollEvents();
	
	// OpenGL clear and draw
	glClearColor(gl->clearColour.r, gl->clearColour.g, gl->clearColour.b, gl->clearColour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	
	// Enable depth tests
	glEnable(GL_DEPTH_TEST);
	
	// Use the first progam
	glUseProgram(gl->programs[0]);
	
	// Calculate the projection matrix
	// We use the current window width and height to calculate the aspect ratio
	// for each frame, allowing dynamic resize of the window.
	int w, h;
	glfwGetWindowSize(gl->window, &w, &h);
	DgMat4 proj = DgMat4NewPerspective2(0.125f, (float) w / (float) h, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "proj"), 1, GL_TRUE, &proj.ax);
	
	// Calculate view matrix (camera transform)
	DgMat4 camera;
	
	// Do the camera
	if (gl->cam_active[0] != 0) {
		uint32_t tid = gl->cam_active[0] - 1;
		camera = DgTransfromBasicCamera(world->trans[tid].pos, world->trans[tid].rot);
	}
	else {
		camera = DgTransformLookAt2(DgVec3New(0.0f, 1.0f, 3.0f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(0.0f, 1.0f, 0.0f));
	}
	
	// Push our matris to the GPU
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	
	for (size_t i = 0; i < world->mesh_count; i++) {
		uint32_t id = world->mesh[i].base.id;
		C_Mesh *mesh = &world->mesh[i];
		
		// If there have been on vericies pushed to the GPU
		if (!mesh->vert_count || !mesh->index_count) {
			continue;
		}
		
		// If the mesh is updated and the allocation of the mesh data failed
		if ((!mesh->index || !mesh->vert) && mesh->updated) {
			continue;
		}
		
		// Push new verticies if needed
		if (mesh->updated) {
			if (!mesh->vbo) {
				glGenBuffers(1, &mesh->vbo);
			}
			
			if (!mesh->ebo) {
				glGenBuffers(1, &mesh->ebo);
			}
			
			if (!mesh->vao) {
				glGenVertexArrays(1, &mesh->vao);
			}
			
			gl_error_check(__FILE__, __LINE__);
			
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
			
			gl_error_check(__FILE__, __LINE__);
			
			glBufferData(GL_ARRAY_BUFFER, mesh->vert_count * 32, mesh->vert, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(uint32_t), mesh->index, GL_STATIC_DRAW);
			
			gl_error_check(__FILE__, __LINE__);
			
			gl_set_format(gl);
			
			mesh->updated = false;
			
			DgFree(mesh->vert);
			mesh->vert = NULL;
			
			DgFree(mesh->index);
			mesh->index = NULL;
			
			gl_error_check(__FILE__, __LINE__);
		}
		
		// Bind the currently active textures for this shader
		const char * texture_name = mesh->texture;
		if (!texture_name) {
			texture_name = "placeholder";
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gltexture_get_name(&gl->texture, texture_name));
		
		glBindVertexArray(mesh->vao);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
		
		gl_error_check(__FILE__, __LINE__);
		
		// Find the transform
		DgVec3 translate = DgVec3New(0.0f, 0.0f, 0.0f);
		DgVec3 rotate = DgVec3New(0.0f, 0.0f, 0.0f);
		DgVec3 scale = DgVec3New(1.0f, 1.0f, 1.0f);
		
		for (int i = 0; i < world->trans_count; i++) {
			if (world->trans[i].base.id == id) {
				translate = world->trans[i].pos;
				rotate = world->trans[i].rot;
				scale = world->trans[i].scale;
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
		
		gl_error_check(__FILE__, __LINE__);
		
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
		
		gl_error_check(__FILE__, __LINE__);
	}
	
	// Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
	// Check for errors
	gl_error_check(__FILE__, __LINE__);
}

void gl_handle_input(GraphicsSystem* gl) {
	/*
	 * Handles most game input, for example keyboard input
	 */
	
	if (glfwGetKey(gl->window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(gl->window, GL_TRUE);
	}
	
	if (glfwGetKey(gl->window, GLFW_KEY_Q)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void graphics_free(GraphicsSystem* gl) {
	/**
	 * Free the graphics subsystem.
	 */
	
	glfwTerminate();
	
	for (uint32_t i = 0; i < gl->programs_count; i++) {
		glDeleteProgram(gl->programs[i]);
	}
	
	gltexture_free(&gl->texture);
	
	DgFree(gl->shaders);
}

bool get_should_keep_open(GraphicsSystem *info) {
	/**
	 * Return true if the window should be kept open.
	 */
	
	return !glfwWindowShouldClose(info->window);
}

void graphics_set_background(GraphicsSystem * restrict gl, const DgVec4 colour) {
	/**
	 * Setter function for the graphics system's background clear colour.
	 */
	
	gl->clearColour = colour;
}

void graphics_set_camera(GraphicsSystem * restrict gl, const uint32_t name) {
	/*
	 * Setter function for the world's active camera.
	 * 
	 * Implementation note: We add one to I so that we can check if no camera
	 * has been set and use default matrix in that case.
	 */
	
	if (!gl) {
		gl = QR_ACTIVE_GRPAHICS_SYSTEM;
	}
	
	for (size_t i = 0; i < gl->world->trans_count; i++) {
		if (gl->world->trans[i].base.id == name) {
			gl->cam_active[0] = i + 1;
			break;
		}
	}
	
	gl->cam_active[2] = name;
}

uint32_t graphics_get_camera(GraphicsSystem * restrict gl) {
	/**
	 * Getter function for the active camera's ID.
	 */
	
	return gl->cam_active[2];
}
