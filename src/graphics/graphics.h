/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the graphics core.
 */

#pragma once

#include <vulkan/vulkan.h>

#include "../math/vector2.h"
#include "../math/vector3.h"

typedef struct DgVulkanInfo {
	// Vulkan instance
	VkInstance instance;
	VkApplicationInfo app_info;
	VkInstanceCreateInfo inst_info;
	
	// Devices
	VkDevice device;
	VkPhysicalDevice *devices;
	uint32_t device_count;
	VkDeviceCreateInfo device_info;
	
	// Queues
	VkDeviceQueueCreateInfo queue_info;
	VkQueueFamilyProperties *queues;
	uint32_t queue_count;
	uint32_t graphics_queue_index;
	
	// Command Buffers
	VkCommandPool cmdpool;
	VkCommandBuffer* cmdbufs;
	VkCommandPoolCreateInfo cmdpool_info;
	VkCommandBufferAllocateInfo cmdbuf;
} DgVulkanInfo;

DgVulkanInfo* graphics_init();
void graphics_free();
