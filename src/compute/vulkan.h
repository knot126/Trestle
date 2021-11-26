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
void vulkan_print_device_properties(uint32_t count, VkPhysicalDevice *devices);
VkPhysicalDevice vulkan_get_best_device_handle(uint32_t count, VkPhysicalDevice *devices);
VkResult vulkan_enumerate_queue_family_properties(VkPhysicalDevice device, uint32_t *prop_count, VkQueueFamilyProperties **prop);
void vulkan_print_queue_family_properties(uint32_t count, VkQueueFamilyProperties *properties);

VkResult vulkan_create_logical_device(VkPhysicalDevice physical_device, VkDevice *device, uint32_t queue_count, VkQueueFamilyProperties *queue, uint32_t extension_count, const char ** extension);
void vulkan_free_logical_device(VkDevice device);
