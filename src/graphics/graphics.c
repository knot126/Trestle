/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Basic Graphics Functions, to be called funcs in src/game.c
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#if !defined(DG_GLEW_INCLUDED_OK)
	#include "../glew/glew.h"
	#define DG_GLEW_INCLUDED_OK
#endif
#include <GLFW/glfw3.h>

#include "../graphics/vulkan.h"
#include "../graphics/opengl.h"
#include "../util/alloc.h"
#include "../io/load.h"

DgVulkanInfo* graphics_init(void) {
	DgVulkanInfo* vk = (DgVulkanInfo *) DgAlloc(sizeof(DgVulkanInfo));
	
	if (!vk) {
		printf("Failed to allocate memory for vulkan information.\n");
	}
	
	DgCreateVkInstance(vk);
	DgEnumerateVulkanDevices(vk);
	DgInitialiseVulkanDevice(vk);
	DgCheckForGraphicsOnDevice(vk);
	DgCreateVulkanDevice(vk);
	DgCreateVulkanCommandPool(vk);
	DgCreateVulkanCommandBuffer(vk);
	
	return vk;
}

void graphics_free(DgVulkanInfo* vk) {
	// Free vulkan-related things
	vkFreeCommandBuffers(vk->device, vk->cmd_pool, 1, vk->cmd_buffers);
	vkDestroyCommandPool(vk->device, vk->cmd_pool, NULL);
	vkDestroyDevice(vk->device, NULL);
	vkDestroyInstance(vk->instance, NULL);
	
	// Free temporary tables/arrays
	DgFree(vk->queues);
	DgFree(vk->devices);
	DgFree(vk->cmd_buffers);
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
	
	gl->window = glfwCreateWindow(800, 600, "Decent Games Engine", NULL, NULL);
	
	if (!gl->window) {
		printf("Failed to create glfw window.\n");
		exit(-1);
	}
	
	glfwMakeContextCurrent(gl->window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Create a VAO
	glGenVertexArrays(1, &gl->vao);
	glBindVertexArray(gl->vao);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Vertex datas
	const float g_Triangle[] = {
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 1.0f, -1.0f, 0.0f,
	};
	
	// Making a buffer
	glGenBuffers(1, &gl->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Triangle), g_Triangle, GL_STATIC_DRAW);
	
	gl_error_check(__FILE__, __LINE__);
	
	// Load shaders
	gl->shaders = DgAlloc(sizeof(GLuint *) * 2);
	
	GLuint vertex_shader = gl_load_shader("assets://shaders/vertex.glsl", GL_VERTEX_SHADER);
	if (!vertex_shader) { exit(EXIT_FAILURE); }
	gl->shaders[0] = vertex_shader;
	
	GLuint fragment_shader = gl_load_shader("assets://shaders/frag.glsl", GL_FRAGMENT_SHADER);
	if (!fragment_shader) { exit(EXIT_FAILURE); }
	gl->shaders[1] = fragment_shader;
	
	gl->shader_count = 2;
	
	// Create program and add shaders to it
	gl->program = glCreateProgram();
	glAttachShader(gl->program, vertex_shader);
	glAttachShader(gl->program, fragment_shader);
	glBindFragDataLocation(gl->program, 0, "outColor");
	glLinkProgram(gl->program);
	glUseProgram(gl->program);
	
	gl_error_check(__FILE__, __LINE__);
	
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
	
	for (int i = 0; i < gl->shader_count; i++) {
		glDeleteShader(gl->shaders[i]);
	}
	
	glDeleteBuffers(1, &gl->vbo);
	glDeleteVertexArrays(1, &gl->vao);
	
	DgFree(gl->shaders);
	DgFree(gl);
}
