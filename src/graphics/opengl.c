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
//#include "mgmt_texture.c"

#include "opengl.h"

// Yes, it's odd, but I really rather not include the header files in this file.
#include "glutils.h"

void gl_set_window_size(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
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
		DgLog(DG_LOG_FATAL, "Failed to load GLAD");
		return NULL;
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
		DgLog(DG_LOG_ERROR, "Could not set window icon.");
	}
	
	// gl_error_check(__FILE__, __LINE__);
	
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
	*/
	
	// Load shaders
	uint32_t res = graphicsLoadShader(gl, "assets://shaders/main.glsl");
	if (res) {
		// failed to load shader
		// if 2 the error affects other parts too and is fatal
	}
	
	res = graphicsLoadShader(gl, "assets://shaders/text.glsl");
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
	
	// Making textures
	DgBitmap *bmp = DgBitmapGenTiles(256, 256, 128);
	if (bmp) {
		graphicsLoadTextureFromBuffer(gl, bmp, GL_TEXTURE0);
		DgBitmapFree(bmp);
	}
	
	graphicsLoadTextureFromFile(gl, "assets://gfx/font.png", GL_TEXTURE1);
	
	gl_error_check(__FILE__, __LINE__);
	
	glUseProgram(gl->programs[0]);
	glUniform1i(glGetUniformLocation(gl->programs[0], "image"), 0);
	
	gl_error_check(__FILE__, __LINE__);
	
	glUseProgram(gl->programs[1]);
	glUniform1i(glGetUniformLocation(gl->programs[1], "font"), 1);
	
	gl_error_check(__FILE__, __LINE__);
	
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
	if (world->cam_active[0] != 0) {
		uint32_t tid = world->cam_active[0] - 1, cid = world->cam_active[1] - 1;
		camera = DgTransfromBasicCamera(world->trans[tid].pos, world->trans[tid].rot);
	}
	else {
		camera = DgTransformLookAt2(DgVec3New(0.0f, 1.0f, 3.0f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(0.0f, 1.0f, 0.0f));
	}
	
	// Push our matris to the GPU
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	
	// Bind the currently active textures for this shader
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, gl->textures[0]);
	
	for (size_t i = 0; i < world->mesh_count; i++) {
		uint32_t id = world->mesh[i].base.id;
		C_Mesh *mesh = &world->mesh[i];
		
		if (!mesh->vert_count || !mesh->index_count || !mesh->vert || !mesh->index) {
			continue;
		}
		
		// Push new verticies if needed
		if (world->mesh[i].updated) {
			if (!world->mesh[i].vbo) {
				glGenBuffers(1, &world->mesh[i].vbo);
			}
			
			if (!world->mesh[i].ebo) {
				glGenBuffers(1, &world->mesh[i].ebo);
			}
			
			if (!world->mesh[i].vao) {
				glGenVertexArrays(1, &world->mesh[i].vao);
			}
			
			gl_error_check(__FILE__, __LINE__);
			
			glBindVertexArray(world->mesh[i].vao);
			glBindBuffer(GL_ARRAY_BUFFER, world->mesh[i].vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->mesh[i].ebo);
			
			gl_error_check(__FILE__, __LINE__);
			
			glBufferData(GL_ARRAY_BUFFER, world->mesh[i].vert_count * 32, world->mesh[i].vert, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, world->mesh[i].index_count * sizeof(uint32_t), world->mesh[i].index, GL_STATIC_DRAW);
			
			gl_error_check(__FILE__, __LINE__);
			
			gl_set_format(gl);
			
			world->mesh[i].updated = false;
			
			gl_error_check(__FILE__, __LINE__);
		}
		
		glBindVertexArray(world->mesh[i].vao);
		glBindBuffer(GL_ARRAY_BUFFER, world->mesh[i].vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->mesh[i].ebo);
		
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
		
		glDrawElements(GL_TRIANGLES, world->mesh[i].index_count, GL_UNSIGNED_INT, 0);
		
		gl_error_check(__FILE__, __LINE__);
	}
	
	// Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
	glUseProgram(gl->programs[1]);
	
	float screen_ratio = (float) h / (float) w;
	
	// Make sure the UI world exsists and that there is at least more than one box
	if (world->ui && world->ui->text_count > 0) {
		// ~= foreach ( C_UIText element : world->ui->text )
		for (uint32_t i = 0; i < world->ui->text_count; i++) {
			uint32_t id = world->ui->text[i].base.id;
			C_UIText *element = &world->ui->text[i];
			
			if (!element->text || element->size <= 0.0f) {
				continue;
			}
			
			// Update the text if it was changed last frame in some ways
			if (element->updated) {
				// Create buffers
				if (!element->vbo) {
					glGenBuffers(1, &element->vbo);
				}
				
				if (!element->ebo) {
					glGenBuffers(1, &element->ebo);
				}
				
				if (!element->vao) {
					glGenVertexArrays(1, &element->vao);
				}
				
				glBindVertexArray(element->vao);
				glBindBuffer(GL_ARRAY_BUFFER, element->vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->ebo);
				
				gl_error_check(__FILE__, __LINE__);
				
				// Compute new vertex data...
				uint32_t string_len = strlen(element->text);
				
				element->vertex_count = string_len * 4;
				
				if (element->vertex) {
					DgFree(element->vertex);
				}
				
				element->vertex = (float *) DgAlloc(element->vertex_count * sizeof(float) * 4);
				
				if (!element->vertex) {
					DgLog(DG_LOG_ERROR, "Failed to allocate memory for vertex cache.");
					continue;
				}
				
				// Allocate for index data...
				element->index_count = string_len * 6;
				
				if (element->index) {
					DgFree(element->index);
				}
				
				element->index = DgAlloc(element->index_count * sizeof(uint32_t));
				
				if (!element->index) {
					DgLog(DG_LOG_ERROR, "Failed to allocate memory for index cache.");
				}
				
				// NOTE: Here is where we make the vertex data...
				const float size = element->size;
				const DgVec2 pos = element->pos;
				DgVec2 next = pos;
				
				for (uint32_t c = 0; c < string_len; c++) {
					float tex_u = (float) ((element->text[c] % 16) / 16.0f);
					float tex_v = 1.0f - ((element->text[c] / 16) / 8.0f);
					
					element->vertex[(c * 16) + 0] = next.x;
					element->vertex[(c * 16) + 1] = next.y;
					element->vertex[(c * 16) + 2] = tex_u;
					element->vertex[(c * 16) + 3] = tex_v;
					
					element->vertex[(c * 16) + 4] = next.x;
					element->vertex[(c * 16) + 5] = next.y - size;
					element->vertex[(c * 16) + 6] = tex_u;
					element->vertex[(c * 16) + 7] = tex_v - (1.0f / 8.0f);
					
					element->vertex[(c * 16) + 8] = next.x + (size / 2.0f * screen_ratio);
					element->vertex[(c * 16) + 9] = next.y - size;
					element->vertex[(c * 16) + 10] = tex_u + (1.0f / 16.0f);
					element->vertex[(c * 16) + 11] = tex_v - (1.0f / 8.0f);
					
					element->vertex[(c * 16) + 12] = next.x + (size / 2.0f * screen_ratio);
					element->vertex[(c * 16) + 13] = next.y;
					element->vertex[(c * 16) + 14] = tex_u + (1.0f / 16.0f);
					element->vertex[(c * 16) + 15] = tex_v;
					
					element->index[(c * 6) + 0] = 0 + (c * 4);
					element->index[(c * 6) + 1] = 1 + (c * 4);
					element->index[(c * 6) + 2] = 2 + (c * 4);
					element->index[(c * 6) + 3] = 0 + (c * 4);
					element->index[(c * 6) + 4] = 2 + (c * 4);
					element->index[(c * 6) + 5] = 3 + (c * 4);
					
					next.x += (size / 2.0f * screen_ratio);
				}
				
				// Push the data to the GPU
				glBufferData(GL_ARRAY_BUFFER, element->vertex_count * sizeof(float) * 4, element->vertex, GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, element->index_count * sizeof(uint32_t), element->index, GL_STATIC_DRAW);
				
				gl_error_check(__FILE__, __LINE__);
				
				// Specify how OpenGL should get the data
				GLuint attr;
				
				attr = glGetAttribLocation(gl->programs[1], "position");
				glVertexAttribPointer(
					attr,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(float) * 4,
					(void *) 0
				);
				glEnableVertexAttribArray(attr);
				
				attr = glGetAttribLocation(gl->programs[1], "texpos");
				glVertexAttribPointer(
					attr,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(float) * 4,
					(void *) (2 * sizeof(float))
				);
				glEnableVertexAttribArray(attr);
				
				gl_error_check(__FILE__, __LINE__);
				
				element->updated = false;
			}
			
			// And of course draw at the end...
			
			glBindVertexArray(element->vao);
			glBindBuffer(GL_ARRAY_BUFFER, element->vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element->ebo);
			
			if (element->index_count == 0) {
				DgLog(DG_LOG_WARNING, "Text {i = %d} does not have any vertex indicies...", i);
				continue;
			}
			
			glDrawElements(GL_TRIANGLES, element->index_count, GL_UNSIGNED_INT, 0);
			
			gl_error_check(__FILE__, __LINE__);
		}
	}
	
	// Unbind everything
	glBindVertexArray(0);
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
	
	for (uint32_t i = 0; i < gl->programs_count; i++) {
		glDeleteProgram(gl->programs[i]);
	}
	
	DgFree(gl->textures);
	DgFree(gl->shaders);
	DgFree(gl);
}

bool gl_get_should_keep_open(DgOpenGLContext *info) {
	return !glfwWindowShouldClose(info->window);
}
