/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan low-level utilities
 */

#include <string.h>
#include "misc/grpreq.h"
#include "util/log.h"
#include "util/alloc.h"
#include "util/str.h"
#include "util/array.h"

#include "vulkan.h"

DG_CREATE_ARRAY_DEFS(VulkanString, const char *, uint32_t);
DG_CREATE_ARRAY_DEFS(VkPhysicalDevice, VkPhysicalDevice, uint32_t);

// =============================================================================
// Instance
// =============================================================================

VkResult vulkan_instance_init(
	VkInstance *instance,
	uint32_t *version,
	VkApplicationInfo *application_info,
	VkInstanceCreateInfo *instance_create_info,
	uint32_t extension_count, const char **extensions,
	uint32_t layer_count, const char **layers)
{
	/**
	 * Initialise a new vulkan instance.
	 * 
	 *  - instance: Pointer to an unused VkInstance object.
	 *  - version: uint32_t that the Vulkan version will be written to, or NULL not to check.
	 *  - application_info: A pointer to the VkApplicationInfo structure to allocate.
	 *  - instance_create_info: A pointer to the VkInstanceCreateInfo structure to allocate.
	 *  - extensions: List of extenstions to enable, or 0 and NULL.
	 *  - layers: List of layers to enable, or 0 and NULL.
	 * 
	 * Returns the same code as the first error or VK_SUCCESS.
	 */
	
	VkResult status;
	
	// Get instance version
	if (version) {
		status = vkEnumerateInstanceVersion(version);
		
		if (status != VK_SUCCESS) {
			return status;
		}
	}
	
	// Create the vulkan instance
	*application_info = (VkApplicationInfo) {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = "",
		.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
		.pEngineName = "",
		.engineVersion = VK_MAKE_VERSION(0, 0, 0),
		.apiVersion = VK_MAKE_VERSION(1, 1, 0),
	};
	
	*instance_create_info = (VkInstanceCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = application_info,
		.enabledLayerCount = layer_count,
		.ppEnabledLayerNames = layers,
		.enabledExtensionCount = extension_count,
		.ppEnabledExtensionNames = extensions,
	};
	
	status = vkCreateInstance(instance_create_info, NULL, instance);
	
	if (status != VK_SUCCESS) {
		return status;
	}
	
	return VK_SUCCESS;
}

void vulkan_instance_free(VkInstance instance) {
	/**
	 * Free a vulkan instance
	 */
	
	vkDestroyInstance(instance, NULL);
}

// =============================================================================
// Physical and virtual devices
// =============================================================================

VkResult vulkan_enumerate_devices(
	VkInstance instance,
	uint32_t *physical_device_count,
	VkPhysicalDevice **physical_device)
{
	/**
	 * Retrive a list of physical devices in the system.
	 * 
	 *  * `instance`: The instance to enumerate physical devices for.
	 *  * `physical_device_count`: Pointer to the uint32_t storing physical device counts.
	 *  * `physical_device`: Pointer to an array of vulkan physical devices.
	 */
	
	vkEnumeratePhysicalDevices(instance, physical_device_count, NULL);
	
	*physical_device = DgAlloc(sizeof **physical_device * *physical_device_count);
	
	if (!physical_device) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	vkEnumeratePhysicalDevices(instance, physical_device_count, *physical_device);
	
	return VK_SUCCESS;
}
