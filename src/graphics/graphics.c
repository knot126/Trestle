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

// TODO: Move these to their own struct...
static VkInstance vk_instance;
static VkApplicationInfo app_info;
static VkInstanceCreateInfo inst_info;
static VkDeviceQueueCreateInfo queue_info;

void graphics_init() {
	VkResult vk_status;
	uint32_t device_count = 0;
	uint32_t queue_count = 0;
	VkPhysicalDevice *devices;
	VkQueueFamilyProperties *queues;
	
	// Clear instance
	memset(&vk_instance, 0, sizeof(VkInstance));
	
	// Create app info structure
	memset(&app_info, 0, sizeof(VkApplicationInfo));
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "Quick Run";
	app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	app_info.pEngineName = "Decent Games Engine";
	app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;
	
	// Create instance info structure
	
	memset(&inst_info, 0, sizeof(VkInstanceCreateInfo));
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = NULL;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = NULL;
	
	// Create instance
	vk_status = vkCreateInstance(&inst_info, NULL, &vk_instance);
	
	if (vk_status == VK_ERROR_INCOMPATIBLE_DRIVER) {
		printf("Error: This video card or its driver does not support Vulkan or a required extension.\n");
	}
	else if (vk_status) {
		printf("Error: Unknown error creating Vulkan instance. Abort.\n");
	}
	
	// Enumerate devices
	vk_status = vkEnumeratePhysicalDevices(vk_instance, &device_count, NULL);
	
	if (device_count < 1) {
		printf("Error: There are no devices.\n");
	}
	
	devices = (VkPhysicalDevice *) DgAlloc(sizeof(VkPhysicalDevice) * device_count);
	
	if (!devices) {
		printf("Error: Failed to allocate memory for devices table.\n");
	}
	
	vk_status = vkEnumeratePhysicalDevices(vk_instance, &device_count, devices);
	
	// Initialise device
	vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queue_count, NULL);
	
	if (queue_count < 1) {
		printf("Error: Less than one queue on selected device(s).\n");
	} else {
		printf("Info: Have %d vulkan queues.\n", queue_count);
	}
	
	queues = (VkQueueFamilyProperties *) DgAlloc(sizeof(VkQueueFamilyProperties) * queue_count);
	
	if (!queues) {
		printf("Error: Failed to allocate memory for queues table.\n");
	}
	
	vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queue_count, queues);
	
	// Check that at least some queues are for graphics
	bool found_graphics = false;
	uint32_t graphics_queue_index = 0;
	
	for (uint32_t i = 0; i < queue_count; i++) {
		if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			found_graphics = true;
			graphics_queue_index = i;
			break;
		}
	}
	
	if (!found_graphics) {
		printf("Error: Graphics not found.\n");
	} else {
		printf("Found queue capable of graphics at index %d.\n", graphics_queue_index);
	}
	
	memset(&queue_info, 0, sizeof(VkDeviceQueueCreateInfo));
	float queue_priority[1] = {0.0};
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = NULL;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = queue_priority;
	
	// Free temporary tables/arrays
	DgFree(queues);
	DgFree(devices);
}

void graphics_free() {
	vkDestroyInstance(vk_instance, NULL);
}
