/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan Communication Module
 */

#pragma once

#include "misc/grpreq.h"
#include "util/array.h"

DG_CREATE_ARRAY(VulkanString, const char *, uint32_t);
DG_CREATE_ARRAY(VkPhysicalDevice, VkPhysicalDevice, uint32_t);

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

VkResult vulkan_init(VulkanSocket *this, uint32_t extension_count, const char **extensions, uint32_t layer_count, const char **layers);
VkResult vulkan_init_simple(VulkanSocket *this);
void vulkan_free(VulkanSocket *this);
