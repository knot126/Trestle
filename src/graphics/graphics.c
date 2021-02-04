/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the graphics core.
 */

#include <inttypes.h>
#include <stdio.h>

#include <vulkan/vulkan.h>

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
	glfwInit();
	
	return 1;
}

void gl_graphics_free(DgOpenGLContext* gl) {
	glfwTerminate();
}
