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
	VkInstance instance;
	VkApplicationInfo app_info;
	VkInstanceCreateInfo inst_info;
	VkDeviceQueueCreateInfo queue_info;
	VkDevice device;
	VkCommandPool cmdpool;
	VkCommandBuffer* cmdbufs;
	VkCommandPoolCreateInfo cmdpool_info;
	VkCommandBufferAllocateInfo cmdbuf;
	VkPhysicalDevice *devices;
	VkQueueFamilyProperties *queues;
	VkDeviceCreateInfo device_info;
	uint32_t device_count;
	uint32_t queue_count;
	uint32_t graphics_queue_index;
} DgVulkanInfo;

DgVulkanInfo* graphics_init();
void graphics_free();
