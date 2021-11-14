/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan Socket - Higher level connection to Vulkan API
 */

#pragma once

#include <inttypes.h>
#include "misc/grpreq.h"

/**
 * VulkanSocket
 * ============
 * 
 * Connection to the Vulkan API 
 */
typedef struct VulkanSocket {
	// Instance
	VkInstance instance;
	uint32_t version;
	const char **layer;
	uint32_t layer_count;
	const char **extension;
	uint32_t extension_count;
	VkInstanceCreateInfo instance_create_info;
	VkApplicationInfo application_info;
	
	// Devices
	VkPhysicalDevice *physical_device;
	uint32_t physical_device_count;
} VulkanSocket;
