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

#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../util/alloc.h"

#include "../graphics/graphics.h"

static VkInstance vk_instance;

void graphics_init() {
	VkResult vk_status;
	uint32_t device_count = 0;
	VkPhysicalDevice *devices;
	
	// Clear instance
	memset(&vk_instance, 0, sizeof(VkInstance));
	
	// Create app info structure
	VkApplicationInfo app_info;
	memset(&app_info, 0, sizeof(VkApplicationInfo));
	
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "Quick Run";
	app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	app_info.pEngineName = "Decent Games Engine";
	app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;
	
	// Create instance info structure
	VkInstanceCreateInfo inst_info;
	memset(&inst_info, 0, sizeof(VkInstanceCreateInfo));
	
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pApplicationInfo = &app_info;
	inst_info.pNext = NULL;
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
	
	// Initialise the device
	
	DgFree(devices);
}

void graphics_free() {
	printf("Destroying graphics subsystem...\n");
	vkDestroyInstance(&vk_instance, NULL);
}
