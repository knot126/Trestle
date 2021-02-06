/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the graphics core.
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include "../glew/glew.h"

#include "../graphics/vulkan.h"
#include "../graphics/opengl.h"
#include "../util/alloc.h"

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
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	gl->window = glfwCreateWindow(800, 600, "Decent Games Engine", NULL, NULL);
	
	if (!gl->window) {
		printf("Failed to create glfw window.\n");
		exit(-1);
	}
	
	glfwMakeContextCurrent(gl->window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Vertex datas
	const float g_Triangle[] = {
		 0.0f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f,
	};
	
	// Making a buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Triangle), g_Triangle, GL_STATIC_DRAW);
	
	return gl;
}

void gl_graphics_free(DgOpenGLContext* gl) {
	glfwTerminate();
	
	DgFree(gl);
}
