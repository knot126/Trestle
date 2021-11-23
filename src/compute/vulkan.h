/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan low-level utilities
 */

#pragma once

#include "misc/grpreq.h"
#include "util/array.h"

DG_CREATE_ARRAY(VulkanString, const char *, uint32_t);
DG_CREATE_ARRAY(VkPhysicalDevice, VkPhysicalDevice, uint32_t);

VkResult vulkan_instance_init(
	VkInstance *instance,
	uint32_t *version,
	VkApplicationInfo *application_info,
	VkInstanceCreateInfo *instance_create_info,
	uint32_t extension_count, const char **extensions,
	uint32_t layer_count, const char **layers);
void vulkan_instance_free(VkInstance instance);
VkResult vulkan_enumerate_devices(
	VkInstance instance,
	uint32_t *physical_device_count,
	VkPhysicalDevice **physical_device);
