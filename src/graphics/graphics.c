/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the graphics core.
 */

#include <string.h>
#include <inttypes.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdbool.h>

#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../util/alloc.h"

#include "../graphics/graphics.h"

static void DgCreateVkInstance(DgVulkanInfo* vk) {
	VkResult vk_status;
	
	// Clear instance
	memset(&vk->instance, 0, sizeof(VkInstance));
	
	// Create app info structure
	memset(&vk->app_info, 0, sizeof(VkApplicationInfo));
	vk->app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vk->app_info.pNext = NULL;
	vk->app_info.pApplicationName = "Quick Run";
	vk->app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	vk->app_info.pEngineName = "Decent Games Engine";
	vk->app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	vk->app_info.apiVersion = VK_API_VERSION_1_0;
	
	// Create instance info structure
	memset(&vk->inst_info, 0, sizeof(VkInstanceCreateInfo));
	vk->inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vk->inst_info.pNext = NULL;
	vk->inst_info.flags = 0;
	vk->inst_info.pApplicationInfo = &vk->app_info;
	vk->inst_info.enabledLayerCount = 0;
	vk->inst_info.ppEnabledLayerNames = NULL;
	vk->inst_info.enabledExtensionCount = 0;
	vk->inst_info.ppEnabledExtensionNames = NULL;
	
	// Create instance
	vk_status = vkCreateInstance(&vk->inst_info, NULL, &vk->instance);
	
	if (vk_status == VK_ERROR_INCOMPATIBLE_DRIVER) {
		printf("Error: This video card or its driver does not support Vulkan or a required extension.\n");
	}
	else if (vk_status) {
		printf("Error: Unknown error creating Vulkan instance. Abort.\n");
	}
}

static void DgEnumerateVulkanDevices(DgVulkanInfo* vk) {
	// Enumerate devices
	VkResult vk_status;
	
	vk_status = vkEnumeratePhysicalDevices(vk->instance, &vk->device_count, NULL);
	
	if (vk->device_count < 1) {
		printf("Error: There are no devices.\n");
	}
	
	vk->devices = (VkPhysicalDevice *) DgAlloc(sizeof(VkPhysicalDevice) * vk->device_count);
	
	if (!vk->devices) {
		printf("Error: Failed to allocate memory for devices table.\n");
	}
	
	vk_status = vkEnumeratePhysicalDevices(vk->instance, &vk->device_count, vk->devices);
}

static void DgInitialiseVulkanDevice(DgVulkanInfo* vk) {
	// Initialise device
	VkResult vk_status;
	
	vkGetPhysicalDeviceQueueFamilyProperties(vk->devices[0], &vk->queue_count, NULL);
	
	if (vk->queue_count < 1) {
		printf("Error: Less than one queue on selected device(s).\n");
	} else {
		printf("Info: Have %d vulkan queues.\n", vk->queue_count);
	}
	
	vk->queues = (VkQueueFamilyProperties *) DgAlloc(sizeof(VkQueueFamilyProperties) * vk->queue_count);
	
	if (!vk->queues) {
		printf("Error: Failed to allocate memory for queues table.\n");
	}
	
	vkGetPhysicalDeviceQueueFamilyProperties(vk->devices[0], &vk->queue_count, vk->queues);
}

static void DgCheckForGraphicsOnDevice(DgVulkanInfo* vk) {
	// Check that at least some queues are for graphics
	
	VkResult vk_status;
	bool found_graphics;
	
	for (uint32_t i = 0; i < vk->queue_count; i++) {
		if (vk->queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			found_graphics = true;
			vk->graphics_queue_index = i;
			break;
		}
	}
	
	if (!found_graphics) {
		printf("Error: Graphics not found.\n");
	} else {
		printf("Found queue capable of graphics at index %d.\n", vk->graphics_queue_index);
	}
}

static void DgCreateVulkanDevice(DgVulkanInfo* vk) {
	// Creates a vulkan device
	VkResult vk_status;
	
	memset(&vk->queue_info, 0, sizeof(VkDeviceQueueCreateInfo));
	float queue_priority[1] = {0.0};
	vk->queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vk->queue_info.pNext = NULL;
	vk->queue_info.queueCount = 1;
	vk->queue_info.pQueuePriorities = queue_priority;
	
	memset(&vk->device_info, 0, sizeof(VkDeviceCreateInfo));
	vk->device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vk->device_info.pNext = NULL;
	vk->device_info.queueCreateInfoCount = 1;
	vk->device_info.pQueueCreateInfos = &vk->queue_info;
	vk->device_info.enabledExtensionCount = 0;
	vk->device_info.ppEnabledExtensionNames = NULL;
	vk->device_info.enabledLayerCount = 0;
	vk->device_info.ppEnabledLayerNames = NULL;
	vk->device_info.pEnabledFeatures = NULL;
	
	vk_status = vkCreateDevice(vk->devices[0], &vk->device_info, NULL, &vk->device);
	
	if (vk_status) {
		printf("Error: Failed to create device.\n");
	}
}

static void DgCreateVulkanCommandPool(DgVulkanInfo* vk) {
	// Create a command pool
	VkResult vk_status;
	
	memset(&vk->cmd_pool_info, 0, sizeof(VkCommandPoolCreateInfo));
	vk->cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	vk->cmd_pool_info.pNext = NULL;
	vk->cmd_pool_info.queueFamilyIndex = vk->graphics_queue_index;
	vk->cmd_pool_info.flags = 0;
	
	vk_status = vkCreateCommandPool(vk->device, &vk->cmd_pool_info, NULL, &vk->cmd_pool);
	
	if (vk_status) {
		printf("Error: Failed to create command pool.\n");
	}
}

static void DgCreateVulkanCommandBuffer(DgVulkanInfo* vk) {
	// Create a command buffer
	VkResult vk_status;
	
	memset(&vk->cmd_buffer_info, 0, sizeof(VkCommandBufferAllocateInfo));
	vk->cmd_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	vk->cmd_buffer_info.pNext = NULL;
	vk->cmd_buffer_info.commandPool = vk->cmd_pool;
	vk->cmd_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vk->cmd_buffer_info.commandBufferCount = 1;
	
	vk->cmd_buffers = DgAlloc(sizeof(VkCommandBuffer) * 1);
	
	if (!vk->cmd_buffers) {
		printf("Failed to allocate memory for command buffer.\n");
	}
	
	vk_status = vkAllocateCommandBuffers(vk->device, &vk->cmd_buffer_info, vk->cmd_buffers);
}

DgVulkanInfo* graphics_init() {
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
