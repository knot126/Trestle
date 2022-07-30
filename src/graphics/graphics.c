/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics System
 * 
 * @reviewed 2022-07-10T18:00:00+00:00:00
 * @deprecated Old
 */

/**
 * Graphics system TODO:
 * 
 *   * Refactoring graphics system: a lot of this system was written when I had
 *     just started programming; there are things that can be done much better.
 *   * Abstract away GLFW related things: it would be nice to have a more
 *     generic interface to the windowing and input library.
 *   * Speaking of that, unbind input and graphics system if it is possible at
 *     all.
 *   * Stop making OpenGL calls directly: This is bad for various reasons, 
 *     mainly that there is a very hard dependency on a very much dying library.
 * 
 * And so much more...
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "gl_incl.h"

#include "graphics/vertex3d.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/str.h"
#include "util/bitmap.h"
#include "util/log.h"
#include "util/surface.h"
#include "util/rand.h"
#include "types.h" // For g_deltaTime

#include "graph/graph.h"

#include "texture.h"

#include "graphics.h"

#define GL_ERROR_CHECK() gl_error_check(__FILE__, __LINE__)

enum {
	QR_SURFACE_MAX_SAMPLES = 420,
};

// Yes, it's odd, but I really rather not include the header files in this file.
#include "glutils.h"

void graphics_set_window_size(GLFWwindow* window, int w, int h) {
	/**
	 * GLFW callback to set the window size.
	 */
	
	glViewport(0, 0, w, h);
}

static int32_t graphics_load_default_shaders(GraphicsSystem * restrict gl) {
	/**
	 * Load the default shaders
	 */
	
	uint32_t res;
	
	// The main 3D shader
	res = graphicsLoadShader(gl, "assets://shaders/main.glsl");
	
	if (res != 0) { /// @todo Improve error codes
		DgLog(DG_LOG_ERROR, "Failed to load shader for 3D stuff.");
		return -1;
	}
	
	glUseProgram(gl->programs[0]);
	
	// The main 2D shader
	res = graphicsLoadShader(gl, "assets://shaders/gui.glsl");
	
	if (res != 0) { /// @todo Improve error codes
		DgLog(DG_LOG_ERROR, "Failed to load shader for GUI.");
		return -1;
	}
	
	return 0;
}

void graphics_init(GraphicsSystem * restrict gl) {
	/**
	 * Initialise any global OpenGL graphics state.
	 */
	
	// Create context info structure
	memset(gl, 0, sizeof *gl);
	
	// GLFW init
	glfwInit();
	
	// Window paramaters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	
	// Create window
	char *w_title = DgStrdup("Main Window"); /// @todo free
	int w_width = 1280;
	int w_height = 720;
	
	gl->window = glfwCreateWindow(w_width, w_height, w_title, NULL, NULL);
	
	DgFree(w_title);
	
	if (!gl->window) {
		DgLog(DG_LOG_FATAL, "Failed to create glfw window");
		return;
	}
	
	// Make the OpenGL context current
	glfwMakeContextCurrent(gl->window);
	
	int ret = gladLoadGLLoader( (GLADloadproc) &glfwGetProcAddress );
	if (!ret) {
		DgLog(DG_LOG_FATAL, "Failed to load GLAD");
		return;
	}
	
	// Unlock framerate
	glfwSwapInterval(0);
	
	// Regiser window resize callback
	glfwSetFramebufferSizeCallback(gl->window, &graphics_set_window_size);
	
	if (graphics_load_default_shaders(gl) < 0) {
		return;
	}
	
	glUseProgram(gl->programs[0]);
	
	// Check for errors
	GL_ERROR_CHECK();
	
	// Create texture manager
	gltexture_init(&gl->texture);
	
	// Making textures
	DgBitmap bmp;
	
	if (DgBitmapInit(&bmp, 64, 64, 3) == false) {
		DgBitmapFill(&bmp, (DgColour) {1.0, 1.0, 1.0, 1.0});
		gltexture_load_buffer(&gl->texture, "placeholder", &bmp);
		gltexture_set_unit(&gl->texture, "placeholder", GL_TEXTURE0);
		DgBitmapFree(&bmp);
	}
	else {
		DgLog(DG_LOG_ERROR, "Failed to generate default placeholder texture.");
	}
	
	// Load textures from the master list
	gltexture_load_list(&gl->texture, "assets://gfx/textures.xml");
	
	// Setting texture uniforms in shaders
	GL_ERROR_CHECK();
	
	// Set uniforms for 3D shader
	glUseProgram(gl->programs[0]);
	glUniform1i(glGetUniformLocation(gl->programs[0], "image"), 0);
	
	// The very basic lighting model
	glUniform1f(glGetUniformLocation(gl->programs[0], "LightFactor"), 0.15f);
	DgVec3 v = {1.0, 1.0, 0.1};
	v = DgVec3Normalise(v);
	glUniform3f(glGetUniformLocation(gl->programs[0], "LightDirection"), v.x, v.y, v.z);
	GL_ERROR_CHECK();
	
	// Clear current program
	glUseProgram(0);
	
	// Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	GL_ERROR_CHECK();
	
	// Set default curve render quality
	graphics_set_curve_render_quality(gl, 3.0f);
	
	// Set the default FoV
	graphics_set_fov(gl, 0.125f);
	
	// Set default clear colour
	gl->clearColour = (DgVec4) {0.5f, 0.5f, 0.5f, 1.0f};
}

static void graphics_update_mesh(GraphicsSystem * const restrict gl, uint32_t * const restrict vbo, uint32_t * const restrict ebo, uint32_t * const restrict vao, uint32_t vertex_count, QRVertex3D ** vertex, uint32_t index_count, uint32_t ** index, bool * restrict updated) {
	/**
	 * Update a mesh by pushing vertex and index data to OpenGL. vbo, vao and
	 * ebo should be writeable and are used as what they are named respectively.
	 */
	
	if (!*vertex || !*index) {
		DgLog(DG_LOG_ERROR, "Cannot update mesh: vertex (0x%.16X) or index (0x%.16X) pointer is NULL!", *vertex, *index);
		return;
	}
	
	if (!*vbo) {
		glGenBuffers(1, vbo);
	}
	
	if (!*ebo) {
		glGenBuffers(1, ebo);
	}
	
	if (!*vao) {
		glGenVertexArrays(1, vao);
	}
	
	GL_ERROR_CHECK();
	
	glBindVertexArray(*vao);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	
	GL_ERROR_CHECK();
	
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof **vertex, (void *) (*vertex), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof **index, (void *) (*index), GL_STATIC_DRAW);
	
	GL_ERROR_CHECK();
	
	// =================================================================
	// Register vertex attributes
	// =================================================================
	
	GLint attr;
	
	attr = glGetAttribLocation(gl->programs[0], "position");
	
	if (attr < 0) {
		DgLog(DG_LOG_ERROR, "Error: No attribute 'position'.\n", 100);
	}
	
	glVertexAttribPointer(
		attr, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), 
		(void *) 0
	);
	glEnableVertexAttribArray(attr);
	
	GL_ERROR_CHECK();
	
	// =================================================================
	// =================================================================
	
	attr = glGetAttribLocation(gl->programs[0], "texpos");
	
	if (attr < 0) {
		DgLog(DG_LOG_ERROR, "Error: No attribute 'texpos'.\n", 100);
	}
	
	glVertexAttribPointer(
		attr, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 8 * sizeof(float), 
		(void *) (3 * sizeof(float))
	);
	glEnableVertexAttribArray(attr);
	
	GL_ERROR_CHECK();
	
	// =================================================================
	// =================================================================
	
	attr = glGetAttribLocation(gl->programs[0], "colour");
	
	if (attr < 0) {
		DgLog(DG_LOG_ERROR, "Error: No attribute 'colour'.\n", 100);
	}
	
	glVertexAttribPointer(
		attr, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), 
		(void *) (5 * sizeof(float))
	);
	glEnableVertexAttribArray(attr);
	
	GL_ERROR_CHECK();
	
	// =================================================================
	// =================================================================
	
	if (updated) {
		*updated = false;
	}
	
// 	DgLog(DG_LOG_VERBOSE, "Free Vertex");
	DgFree(*vertex);
	*vertex = NULL;
	
// 	DgLog(DG_LOG_VERBOSE, "Free Index");
	DgFree(*index);
	*index = NULL;
	
	GL_ERROR_CHECK();
}

static DgMat4 trCreatePerspectiveMatrixFromPlaneDistances(float left, float right, float bottom, float top, float near, float far) {
	/**
	 * Create a new perspective projection matrix, using left, right, back,
	 * front near and far planes.
	 * 
	 * @note The matrix cannot actually do the perspective divide; this is just
	 * the OpenGL perspective matrix.
	 * @note It is probably easier to understand doing things without using a
	 * matrix. See also scratchapixel.
	 * 
	 * @see https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
	 * 
	 * @param left Left plane distance from zero
	 * @param right Right plane distance from zero
	 * @param bottom Bottom plane distance from zero
	 * @param top Top plane distance from zero
	 * @param near Back plane distance from zero
	 * @param far Front plane distance from zero
	 * @return Matrix representing a perspective projection (w divide not applied)
	 */
	
	DgMat4 proj = DgMat4New(1.0f);
	
	// Projection for the x-coordinate
	proj.ax = (2 * near) / (right - left);
	proj.az = (right + left) / (right - left);
	
	// Projection for the y-coordinate
	proj.by = (2 * near) / (top - bottom);
	proj.bz = (top + bottom) / (top - bottom);
	
	// Divide for z (negative for ... cg "best practises")
	proj.cz = -(far + near) / (far - near);
	proj.cw = -(2 * far * near) / (far - near);
	
	// w-coordinate should now be -z
	proj.dz = -1;
	
	return proj;
}

DgMat4 trCreatePerspectiveMatrix(float fov, float ratio, float near, float far) {
	/**
	 * Create a new perspective projection matrix.
	 * 
	 * @param fov Feild of view
	 * @param ratio Ratio between height and width
	 * @param near Z-value of near plane
	 * @param far Z-value of far plane
	 * @return Matrix representing a perspective projection (w divide not applied)
	 */
	
	float tangent = DgTan(fov / 2);
	float height = near * tangent;
	float width = height * ratio;
	
	return trCreatePerspectiveMatrixFromPlaneDistances(-width, width, -height, height, near, far);
}

void graphics_update(GraphicsSystem * restrict gl, SceneGraph * restrict graph) {
	/*
	 * Update OpenGL-related state and the graphics system
	 */
	
	// Increment frame count
	gl->frame++;
	
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
	DgMat4 proj = trCreatePerspectiveMatrix(gl->camera_fov, (float) w / (float) h, 0.001f, 100.0f);
	//DgMat4 proj = gl->projection_matrix;
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "proj"), 1, GL_TRUE, &proj.ax);
	
	// Calculate view matrix (camera transform)
	DgMat4 camera;
	
	// Do the camera
	if (gl->camera != 0) {
		Transform * const camtrans = graph_get(graph, gl->camera);
		camera = DgTransfromBasicCamera(camtrans->pos, camtrans->rot);
	}
	
	// Push camera matrix to the GPU
	glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "camera"), 1, GL_TRUE, &camera.ax);
	
	for (size_t i = 0; i < gl->mesh_count; i++) {
		Name id = gl->mesh_name[i];
		Mesh *mesh = &gl->mesh[i];
		
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
			graphics_update_mesh(gl, &mesh->vbo, &mesh->ebo, &mesh->vao, mesh->vertex_count, &mesh->vertex, mesh->index_count, &mesh->index, &mesh->updated);
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
		
		GL_ERROR_CHECK();
		
		// Find the transform
		DgVec3 translate = (DgVec3) {0.0f, 0.0f, 0.0f};
		DgVec3 rotate = (DgVec3) {0.0f, 0.0f, 0.0f};
		DgVec3 scale = (DgVec3) {1.0f, 1.0f, 1.0f};
		
		Transform * const trans = graph_get(graph, id);
		
		if (trans) {
			translate = trans->pos;
			rotate = trans->rot;
			scale = trans->scale;
		}
		
		DgMat4 rot_x = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {1.0f, 0.0f, 0.0f}, rotate.x);
		DgMat4 rot_y = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {0.0f, 1.0f, 0.0f}, rotate.y);
		DgMat4 rot_z = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {0.0f, 0.0f, 1.0f}, rotate.z);
		DgMat4 rot_mat = DgMat4ByMat4Multiply(rot_z, DgMat4ByMat4Multiply(rot_y, rot_x));
		
		DgMat4 model = 
			DgMat4ByMat4Multiply(
				DgMat4Translate(DgMat4New(1.0f), translate), 
				DgMat4ByMat4Multiply(rot_mat, 
					DgMat4Scale(DgMat4New(1.0f), scale)
				)
			);
		glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "model"), 1, GL_TRUE, &model.ax);
		
		GL_ERROR_CHECK();
		
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
		
		GL_ERROR_CHECK();
	}
	
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	
	if ((gl->flags & QR_GRAPHICS_DRAW_CURVES_AS_VERTEXISATIONS) == QR_GRAPHICS_DRAW_CURVES_AS_VERTEXISATIONS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	for (size_t i = 0; i < gl->surface_count; i++) {
		Name id = gl->surface_name[i];
		Surface3D *surface = &gl->surface[i];
		
		// If this surface has no objects attached, then continue early.
		if (surface->surface_count <= 0) {
			continue;
		}
		
		// Push new verticies if needed
		if (surface->cache.updated) {
			// TODO: Complete support for multipule surfaces per rendering object
			// NOTE: It could be more efficent to update only the changed parts,
			// but it might only be worth it if (a) the surfaces stay the same
			// quality in redner and (b) they update often, like in an animation.
			// In an editor, I think the impact of updating the full surface would
			// not be so bad.
			
			for (size_t si = 0; si < surface->surface_count; si++) {
				// Calculate sample rate
				DgVec3 size = DgSurface3DGetBoundingBoxSize(&surface->surface[si]);
				
				// Prepare the data
				// FIXME: Curve render quality calculation is flawed becuase it is
				// the literal x and y size, not the u and v size.
				uint32_t samp_x = (uint32_t)(gl->curve_render_quality * (float)size.x);
				uint32_t samp_y = (uint32_t)(gl->curve_render_quality * (float)size.z);
				
				// Don't allow mesh to be over set number of verticies
				if (samp_x > QR_SURFACE_MAX_SAMPLES) {
					samp_x = QR_SURFACE_MAX_SAMPLES;
				}
				
				if (samp_y > QR_SURFACE_MAX_SAMPLES) {
					samp_y = QR_SURFACE_MAX_SAMPLES;
				}
				
				DgVec3 *samples = DgAlloc(samp_x * samp_y * sizeof *samples);
				
				if (!samples) {
					continue;
				}
				
				surface->cache.vertex_count = samp_x * samp_y * 4;
				surface->cache.vertex = DgAlloc(surface->cache.vertex_count * sizeof *(surface->cache.vertex));
				
				if (!surface->cache.vertex) {
					continue;
				}
				
				surface->cache.index_count = samp_x * samp_y * 6;
				surface->cache.index = DgAlloc(surface->cache.index_count * sizeof *(surface->cache.index));
				
				if (!surface->cache.index) {
					continue;
				}
				
				// Calculate the samples data
				for (uint32_t u = 0; u < samp_y; u++) {
					for (uint32_t v = 0; v < samp_x; v++) {
						DgVec3 s = DgSurface3DGetSample(&surface->surface[si], ((float)u) / ((float)(samp_x - 1)), ((float)v) / ((float)(samp_y - 1)));
						samples[(samp_x * u) + v] = s;
					}
				}
				
				// Calculate mesh data
				// We are finding the square that is to the bottom right of the mesh.
				for (uint32_t y = 0; y < (samp_y - 1); y++) {
					for (uint32_t x = 0; x < (samp_x - 1); x++) {
						DgVec3 p00 = samples[(y * samp_x) + x],
							p01 = samples[((y + 1) * samp_x) + x],
							p10 = samples[(y * samp_x) + x + 1],
							p11 = samples[((y + 1) * samp_x) + x + 1];
						
						float r = 0.7f + 0.2f * DgRandFloat();
						DgVec3 c = DgVec3Scale(r, (DgVec3) {gl->clearColour.r, gl->clearColour.g, gl->clearColour.b});
						
						surface->cache.vertex[(y * samp_x * 4) + (x * 4) + 0] = (QRVertex3D) {p00.x, p00.y, p00.z, 0.0f, 0.0f, c.x, c.y, c.z};
						surface->cache.vertex[(y * samp_x * 4) + (x * 4) + 1] = (QRVertex3D) {p01.x, p01.y, p01.z, 0.0f, 0.0f, c.x, c.y, c.z};
						surface->cache.vertex[(y * samp_x * 4) + (x * 4) + 2] = (QRVertex3D) {p10.x, p10.y, p10.z, 0.0f, 0.0f, c.x, c.y, c.z};
						surface->cache.vertex[(y * samp_x * 4) + (x * 4) + 3] = (QRVertex3D) {p11.x, p11.y, p11.z, 0.0f, 0.0f, c.x, c.y, c.z};
						
						surface->cache.index[(y * samp_x * 6) + (x * 6) + 0] = (y * samp_x * 4) + (x * 4) + 0;
						surface->cache.index[(y * samp_x * 6) + (x * 6) + 1] = (y * samp_x * 4) + (x * 4) + 1;
						surface->cache.index[(y * samp_x * 6) + (x * 6) + 2] = (y * samp_x * 4) + (x * 4) + 2;
						surface->cache.index[(y * samp_x * 6) + (x * 6) + 3] = (y * samp_x * 4) + (x * 4) + 1;
						surface->cache.index[(y * samp_x * 6) + (x * 6) + 4] = (y * samp_x * 4) + (x * 4) + 2;
						surface->cache.index[(y * samp_x * 6) + (x * 6) + 5] = (y * samp_x * 4) + (x * 4) + 3;
					}
				}
				
				// Create and update mesh cache
				graphics_update_mesh(gl, &surface->cache.vbo, &surface->cache.ebo, &surface->cache.vao, surface->cache.vertex_count, &surface->cache.vertex, surface->cache.index_count, &surface->cache.index, &surface->cache.updated);
			}
		}
		
		glBindVertexArray(surface->cache.vao);
		glBindBuffer(GL_ARRAY_BUFFER, surface->cache.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface->cache.ebo);
		
		GL_ERROR_CHECK();
		
		// Find the transform
		DgVec3 translate = (DgVec3) {0.0f, 0.0f, 0.0f};
		DgVec3 rotate = (DgVec3) {0.0f, 0.0f, 0.0f};
		DgVec3 scale = (DgVec3) {1.0f, 1.0f, 1.0f};
		
		Transform * const trans = graph_get(graph, id);
		
		if (trans) {
			translate = trans->pos;
			rotate = trans->rot;
			scale = trans->scale;
		}
		
		DgMat4 rot_x = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {1.0f, 0.0f, 0.0f}, rotate.x);
		DgMat4 rot_y = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {0.0f, 1.0f, 0.0f}, rotate.y);
		DgMat4 rot_z = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {0.0f, 0.0f, 1.0f}, rotate.z);
		DgMat4 rot_mat = DgMat4ByMat4Multiply(rot_z, DgMat4ByMat4Multiply(rot_y, rot_x));
		
		DgMat4 model = 
			DgMat4ByMat4Multiply(
				DgMat4Translate(DgMat4New(1.0f), translate), 
				DgMat4ByMat4Multiply(rot_mat, 
					DgMat4Scale(DgMat4New(1.0f), scale)
				)
			);
		glUniformMatrix4fv(glGetUniformLocation(gl->programs[0], "model"), 1, GL_TRUE, &model.ax);
		
		GL_ERROR_CHECK();
		
		glDrawElements(GL_TRIANGLES, surface->cache.index_count, GL_UNSIGNED_INT, 0);
		
		GL_ERROR_CHECK();
	}
// #endif
	
	if ((gl->flags & QR_GRAPHICS_DRAW_CURVES_AS_VERTEXISATIONS) == QR_GRAPHICS_DRAW_CURVES_AS_VERTEXISATIONS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	
	// Disable depth tests
	glDisable(GL_DEPTH_TEST);
	
	// Use the first progam
	glUseProgram(gl->programs[1]);
	
	// Render all of the Mesh2D objects
	for (size_t i = 0; i < gl->mesh2d_count; i++) {
		Name id = gl->mesh2d_name[i];
		Mesh2D *mesh = &gl->mesh2d[i];
		
		// Check that there are actually things to render
		if (!mesh->vertex_count || !mesh->index_count) {
			continue;
		}
		
		// If the mesh is updated or the allocation of the mesh data failed,
		// then just ignore this mesh.
		if (!mesh->index || !mesh->vertex || mesh->updated) {
			continue;
		}
		
		// Update the mesh if needed
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
			
			GL_ERROR_CHECK();
			
			// Bind buffers
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
			
			GL_ERROR_CHECK();
			
			// Push the buffer data
			glBufferData(GL_ARRAY_BUFFER, sizeof *mesh->vertex * mesh->vertex_count, mesh->vertex, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof *mesh->index * mesh->index_count, mesh->index, GL_STATIC_DRAW);
			
			GL_ERROR_CHECK();
			
			// =================================================================
			// Register vertex attributes
			// =================================================================
			
			GLint attr;
			
			attr = glGetAttribLocation(gl->programs[1], "aPos");
			
			if (attr < 0) {
				DgLog(DG_LOG_ERROR, "Error: No attribute 'aPos'.\n", 100);
			}
			
			glVertexAttribPointer(
				attr, 
				2, 
				GL_FLOAT, 
				GL_FALSE, 
				8 * sizeof(float), 
				(void *) 0
			);
			glEnableVertexAttribArray(attr);
			
			GL_ERROR_CHECK();
			
			// =================================================================
			// =================================================================
			
			attr = glGetAttribLocation(gl->programs[1], "aTex");
			
			if (attr < 0) {
				DgLog(DG_LOG_ERROR, "Error: No attribute 'aTex'.\n", 100);
			}
			
			glVertexAttribPointer(
				attr, 
				2, 
				GL_FLOAT, 
				GL_FALSE, 8 * sizeof(float), 
				(void *) (2 * sizeof(float))
			);
			glEnableVertexAttribArray(attr);
			
			GL_ERROR_CHECK();
			
			// =================================================================
			// =================================================================
			
			attr = glGetAttribLocation(gl->programs[1], "aCol");
			
			if (attr < 0) {
				DgLog(DG_LOG_ERROR, "Error: No attribute 'aCol'.\n", 100);
			}
			
			glVertexAttribPointer(
				attr, 
				4, 
				GL_FLOAT, 
				GL_FALSE, 
				8 * sizeof(float), 
				(void *) (4 * sizeof(float))
			);
			glEnableVertexAttribArray(attr);
			
			GL_ERROR_CHECK();
			
			// =================================================================
			// =================================================================
			
			mesh->updated = false;
			
			DgFree(mesh->vertex);
			mesh->vertex = NULL;
			
			DgFree(mesh->index);
			mesh->index = NULL;
			
			GL_ERROR_CHECK();
		}
		
		// Bind the currently active textures for this object
		const char * texture_name = mesh->texture;
		if (!texture_name) {
			texture_name = "placeholder";
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gltexture_get_name(&gl->texture, texture_name));
		
		GL_ERROR_CHECK();
		
		// Bind buffers
		glBindVertexArray(mesh->vao);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
		
		GL_ERROR_CHECK();
		
		// Push the matrix transform
		DgVec3 translate = (DgVec3) {0.0f, 0.0f, 0.0f};
		DgVec3 rotate = (DgVec3) {0.0f, 0.0f, 0.0f};
		DgVec3 scale = (DgVec3) {1.0f, 1.0f, 1.0f};
		
		Transform * const trans = graph_get(graph, id);
		
		if (trans) {
			translate = trans->pos;
			rotate = trans->rot;
			scale = trans->scale;
		}
		
		DgMat4 rot_x = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {1.0f, 0.0f, 0.0f}, rotate.x);
		DgMat4 rot_y = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {0.0f, 1.0f, 0.0f}, rotate.y);
		DgMat4 rot_z = DgMat4Rotate(DgMat4New(1.0f), (DgVec3) {0.0f, 0.0f, 1.0f}, rotate.z);
		DgMat4 rot_mat = DgMat4ByMat4Multiply(rot_z, DgMat4ByMat4Multiply(rot_y, rot_x));
		
		DgMat4 model = 
			DgMat4ByMat4Multiply(
				DgMat4Translate(DgMat4New(1.0f), translate), 
				DgMat4ByMat4Multiply(rot_mat, 
					DgMat4Scale(DgMat4New(1.0f), scale)
				)
			);
		glUniformMatrix4fv(glGetUniformLocation(gl->programs[1], "transform"), 1, GL_TRUE, &model.ax);
		
		GL_ERROR_CHECK();
		
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
	}
	
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	
	// Disable depth tests
	glEnable(GL_DEPTH_TEST);
	
	// Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
	// Check for errors
	GL_ERROR_CHECK();
}

void graphics_free(GraphicsSystem* gl) {
	/**
	 * Free the graphics subsystem.
	 */
	
	for (uint32_t i = 0; i < gl->programs_count; i++) {
		glDeleteProgram(gl->programs[i]);
	}
	
	gltexture_free(&gl->texture);
	
	glfwTerminate();
	
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

void graphics_set_camera(GraphicsSystem * restrict gl, Name name) {
	/*
	 * Setter function for the world's active camera.
	 * 
	 * Implementation note: We add one to I so that we can check if no camera
	 * has been set and use default matrix in that case.
	 */
	
	gl->camera = name;
}

Name graphics_get_camera(GraphicsSystem * restrict gl) {
	/**
	 * Getter function for the active camera's ID.
	 */
	
	return gl->camera;
}

DgVec3 graphics_get_camera_forward(GraphicsSystem * restrict gl, SceneGraph * restrict graph, const DgVec3 *forward) {
	/**
	 * From a vector pointing to the real given direction, retrive a vector
	 * relative to the camera's rotation.
	 */
	
	Transform * const trans = graph_get(graph, gl->camera);
	
	if (!trans) {
		return (DgVec3) {0.0f, 0.0f, 0.0f};
	}
	
	return DgVec3Rotate(*forward, trans->rot);
}

DgVec2I graphics_get_screen_size(GraphicsSystem * restrict gl) {
	/**
	 * The the size of the window.
	 */
	
	DgVec2I size;
	glfwGetWindowSize(gl->window, &size.x, &size.y);
	
	return size;
}

void graphics_set_mouse_disabled(GraphicsSystem * restrict gl, bool enabled) {
	/**
	 * Disable the GLFW mouse controls.
	 */
	
	if (enabled) {
		glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(gl->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	}
	else {
		glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void graphics_set_curve_render_quality(GraphicsSystem * restrict graphics, float quality) {
	/**
	 * Set the render quality of curves in samples per cube unit.
	 */
	
	DgLog(DG_LOG_VERBOSE, "Set curve quality to %.3f", quality);
	
	graphics->curve_render_quality = quality;
}

void graphics_set_fov(GraphicsSystem * restrict graphics, float fov) {
	/**
	 * Set the camera FoV in turns. That is, 1.0 turn = 2pi rad = 360 deg.
	 */
	
	graphics->camera_fov = fov;
}

/**
 * =============================================================================
 * 3D Meshes
 * =============================================================================
 */

static int graphics_realloc_mesh(GraphicsSystem * restrict gl) {
	/**
	 * Take care of reallocating mesh data when low on space. It must make sure
	 * there is at least one free mesh worth of space.
	 */
	
	if (gl->mesh_count >= gl->mesh_alloc) {
		gl->mesh_alloc = 4 + (gl->mesh_alloc * 2);
		gl->mesh = DgRealloc(gl->mesh, sizeof *gl->mesh * gl->mesh_alloc);
		
		if (!gl->mesh) {
			return 1;
		}
		
		gl->mesh_name = DgRealloc(gl->mesh_name, sizeof *gl->mesh_name * gl->mesh_alloc);
		
		if (!gl->mesh_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graphics_find_mesh(GraphicsSystem * restrict gl, Name name) {
	/**
	 * Find a mesh that goes by a given name.
	 */
	
	for (size_t i = 0; i < gl->mesh_count; i++) {
		if (gl->mesh_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graphics_create_mesh(GraphicsSystem * restrict gl, Name name) {
	/**
	 * Creates a mesh entity.
	 */
	
	if (graphics_realloc_mesh(gl)) {
		return 0;
	}
	
	gl->mesh_name[gl->mesh_count++] = name;
	memset(&gl->mesh[gl->mesh_count - 1], 0, sizeof *gl->mesh);
	
	return name;
}

Name graphics_destroy_mesh(GraphicsSystem * restrict gl, Name name) {
	/**
	 * Virtually distroys a mesh, clearing its chunk of memory for later use.
	 * Returns name on successful free, zero on failure to free.
	 */
	
	size_t idx = graphics_find_mesh(gl, name);
	
	if (idx == -1) {
		return 0;
	}
	
	gl->mesh_name[idx] = 0;
	
	Mesh *mesh = &gl->mesh[idx];
	
	if (mesh->vertex) {
		DgFree(mesh->vertex);
	}
	
	if (mesh->index) {
		DgFree(mesh->index);
	}
	
	if (mesh->texture) {
		DgFree((void *) mesh->texture);
	}
	
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ebo);
	glDeleteVertexArrays(1, &mesh->vao);
	
	return name;
}

Name graphics_set_mesh(GraphicsSystem * restrict gl, Name name, size_t vertex_count, QRVertex1 *vertex, size_t index_count, uint32_t *index, const char *texture) {
	/**
	 * Set mesh data for a given mesh.
	 */
	
	size_t loc = graphics_find_mesh(gl, name);
	
	if (loc == -1) {
		return 0;
	}
	
	gl->mesh[loc].vert = (float *) vertex;
	gl->mesh[loc].vert_count = vertex_count;
	gl->mesh[loc].index = index;
	gl->mesh[loc].index_count = index_count;
	gl->mesh[loc].texture = DgStrdup(texture);
	gl->mesh[loc].updated = true;
	
	return name;
}

Mesh * const graphics_get_mesh(GraphicsSystem * restrict gl, Name name) {
	/**
	 * Get a reference to a mesh structure for a given name.
	 */
	
	size_t index = graphics_find_mesh(gl, name);
	
	if (index == -1) {
		return NULL;
	}
	
	return &gl->mesh[index];
}

size_t graphics_get_mesh_counts(GraphicsSystem * restrict gl, size_t *allocsz) {
	/**
	 * Get the count of meshes that are active and optionally the count of meshes
	 * that are allocated.
	 */
	
	if (allocsz) {
		*allocsz = gl->mesh_alloc;
	}
	
	return gl->mesh_count;
}

/**
 * =============================================================================
 * 3D Surfaces
 * =============================================================================
 */

static int graphics_realloc_surface3d(GraphicsSystem * const restrict gl) {
	/**
	 * Reallocate the list of surface entities, making sure there is room for
	 * at least one more surface.
	 */
	
	if (gl->surface_count >= gl->surface_alloc) {
		gl->surface_alloc = 4 + (gl->surface_alloc * 2);
		gl->surface = DgRealloc(gl->surface, sizeof *gl->surface * gl->surface_alloc);
		
		if (!gl->surface) {
			return 1;
		}
		
		gl->surface_name = DgRealloc(gl->surface_name, sizeof *gl->surface_name * gl->surface_alloc);
		
		if (!gl->surface_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graphics_find_surface3d(const GraphicsSystem * const restrict gl, const Name name) {
	/**
	 * Find a surface index given its name.
	 */
	
	for (size_t i = 0; i < gl->surface_count; i++) {
		if (gl->surface_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graphics_create_surface3d(GraphicsSystem * const restrict gl, const Name name) {
	/**
	 * Create a 3D surface.
	 */
	
	if (graphics_realloc_surface3d(gl)) {
		return 0;
	}
	
	gl->surface_name[gl->surface_count] = name;
	memset(&gl->surface[gl->surface_count], 0, sizeof *gl->surface);
	
	gl->surface[gl->surface_count].cache.updated = true;
	
	gl->surface_count++;
	
	return name;
}

Name graphics_add_patch_to_surface3d(GraphicsSystem * const restrict gl, const Name name, DgSurface3D * const restrict patch) {
	/**
	 * Add a single patch to a Surface3D object. This takes the given object, so
	 * internal memory for it must not be freed.
	 */
	
	size_t index = graphics_find_surface3d(gl, name);
	
	if (index == -1) {
		return 0;
	}
	
	Surface3D *surface = &gl->surface[index];
	
	// Reallocate patches
	if (surface->surface_alloc <= surface->surface_count) {
		surface->surface_alloc = 2 + (surface->surface_alloc * 2);
		surface->surface = DgRealloc(surface->surface, sizeof *surface->surface * surface->surface_alloc);
		
		if (!surface->surface) {
			return 0;
		}
	}
	
	// Set new patch
	surface->surface[surface->surface_count] = *patch;
	
	// Increment number of patches
	surface->surface_count++;
	
	// Set as updated
	surface->cache.updated = true;
	
	return name;
}

Name graphics_create_patch_surface3d(GraphicsSystem * const restrict gl, const Name name, const uint32_t x, const uint32_t y, const DgVec3 * const restrict points) {
	/**
	 * Create a surface from the x-count, y-count and points.
	 */
	
	DgSurface3D s;
	
	if (!DgSurface3DInit(&s, x, y)) {
		return 0;
	}
	
	for (uint32_t i = 0; i < x; i++) {
		for (uint32_t j = 0; j < y; j++) {
			DgSurface3DSetPoint(&s, i, j, &points[(x * i) + j]);
		}
	}
	
	if (!graphics_add_patch_to_surface3d(gl, name, &s)) {
		DgLog(DG_LOG_ERROR, "Failed to add patch to 3d object (id = %d)!!", name);
		return 0;
	}
	
	return name;
}

Surface3D * const graphics_get_surface3d(GraphicsSystem * const restrict gl, const Name name) {
	/**
	 * Get a pointer to the surface object indentified by `name`.
	 */
	
	size_t idx = graphics_find_surface3d(gl, name);
	
	if (idx == -1) {
		return NULL;
	}
	
	return &gl->surface[idx];
}

size_t graphics_get_surface3d_counts(const GraphicsSystem * const restrict gl, size_t *allocsz) {
	/**
	 * Return the count of surface3D objects and write the count of memory
	 * allocated to allocsz if not NULL.
	 */
	
	if (allocsz) {
		*allocsz = gl->surface_alloc;
	}
	
	return gl->surface_count;
}

/**
 * =============================================================================
 * 2D Meshes
 * =============================================================================
 */

static int graphics_realloc_mesh2d(GraphicsSystem * restrict gl) {
	/**
	 * Reallocate the list of mesh entities, making sure there is at least one 
	 * free slot available. Returns non-zero when there is an error.
	 */
	
	if (gl->mesh2d_count >= gl->mesh2d_alloc) {
		gl->mesh2d_alloc = 4 + (gl->mesh2d_alloc * 2);
		gl->mesh2d = DgRealloc(gl->mesh2d, sizeof *gl->mesh2d * gl->mesh2d_alloc);
		
		if (!gl->mesh2d) {
			return 1;
		}
		
		gl->mesh2d_name = DgRealloc(gl->mesh2d_name, sizeof *gl->mesh2d_name * gl->mesh2d_alloc);
		
		if (!gl->mesh2d_name) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graphics_find_mesh2d(GraphicsSystem * restrict gl, const Name name) {
	/**
	 * Find a mesh2d's index given its name.
	 */
	
	for (size_t i = 0; i < gl->mesh2d_count; i++) {
		if (gl->mesh2d_name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graphics_create_mesh2d(GraphicsSystem * restrict gl, Name name) {
	/**
	 * Create a 2D mesh object.
	 */
	
	if (graphics_realloc_mesh2d(gl)) {
		return 0;
	}
	
	gl->mesh2d_name[gl->mesh2d_count] = name;
	memset(&gl->mesh2d[gl->mesh2d_count], 0, sizeof *gl->mesh2d);
	
	gl->mesh2d_count++;
	
	return name;
}

Name graphics_set_mesh2d(GraphicsSystem * restrict gl, Name name, uint32_t vertex_count, QRVertex2D *vertex, uint32_t index_count, uint32_t *index, const char *texture) {
	/**
	 * Set the mesh data for the given 2D mesh entity.
	 */
	
	size_t i = graphics_find_mesh2d(gl, name);
	
	if (i == -1) {
		return 0;
	}
	
	Mesh2D *mesh = &gl->mesh2d[i];
	
	mesh->vertex = vertex;
	mesh->vertex_count = vertex_count;
	mesh->index = index;
	mesh->index_count = index_count;
	mesh->texture = texture ? DgStrdup(texture) : NULL;
	mesh->updated = true;
	
	return name;
}

Mesh2D * const graphics_get_mesh2d(GraphicsSystem * restrict gl, Name name) {
	/**
	 * Retrieve a pointer to the mesh data structure.
	 */
	
	size_t i = graphics_find_mesh2d(gl, name);
	
	if (i == -1) {
		return NULL;
	}
	
	return &gl->mesh2d[i];
}

size_t graphics_get_mesh2d_counts(GraphicsSystem * restrict gl, size_t *allocsz) {
	/**
	 * Get the count of 2d meshes that are active and optionally the count of 2d
	 * meshes that are allocated.
	 */
	
	if (allocsz) {
		*allocsz = gl->mesh2d_alloc;
	}
	
	return gl->mesh2d_count;
}

#undef GL_ERROR_CHECK
