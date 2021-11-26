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
// Misc. Utilites
// =============================================================================

uint32_t vulkan_format_version(int part, uint32_t version) {
	/**
	 * Format a vulkan version, where part is variant = 0, major = 1, minor = 2, patch = 3.
	 */
	
	uint32_t r;
	
	switch (part) {
		case 0: {
			r = (uint32_t)((version & 0b1100000000000000000000000000000) >> 29);
			break;
		}
		
		case 1: {
			r = (uint32_t)((version & 0b0011111110000000000000000000000) >> 22);
			break;
		}
		
		case 2: {
			r = (uint32_t)((version & 0b0000000001111111111000000000000) >> 12);
			break;
		}
		
		case 3: {
			r = (uint32_t)((version & 0b0000000000000000000111111111111) >> 0);
			break;
		}
	}
	
	return r;
}

const char *vulkan_stringify_device_type(VkPhysicalDeviceType type) {
	/**
	 * Stringify a vulkan device type.
	 */
	
	switch (type) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER: return "Other";
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated Graphics";
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Descrete Graphics";
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual Graphics";
		case VK_PHYSICAL_DEVICE_TYPE_CPU: return "Generic Processor";
	}
}

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
	
	VkResult status;
	
	if ((status = vkEnumeratePhysicalDevices(instance, physical_device_count, NULL)) != VK_SUCCESS) {
		return status;
	}
	
	*physical_device = DgAlloc(sizeof **physical_device * *physical_device_count);
	
	if (!physical_device) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	if ((status = vkEnumeratePhysicalDevices(instance, physical_device_count, *physical_device)) != VK_SUCCESS) {
		return status;
	}
	
	return VK_SUCCESS;
}

void vulkan_print_device_properties(uint32_t count, VkPhysicalDevice *devices) {
	/**
	 * Print out the properties of all enumerated devices.
	 */
	
	VkPhysicalDeviceProperties device;
	
	for (size_t i = 0; i < count; i++) {
		vkGetPhysicalDeviceProperties(devices[i], &device);
		
		DgLog(DG_LOG_VERBOSE, "Device %d:", i);
		DgLog(DG_LOG_VERBOSE, "\tDevice Name: %s", &device.deviceName);
		DgLog(DG_LOG_VERBOSE, "\tAPI Version: %d.%d.%d (variant %d)", vulkan_format_version(1, device.apiVersion), vulkan_format_version(2, device.apiVersion), vulkan_format_version(3, device.apiVersion), vulkan_format_version(0, device.apiVersion));
		DgLog(DG_LOG_VERBOSE, "\tDriver Version: %d.%d.%d (variant %d)", vulkan_format_version(1, device.driverVersion), vulkan_format_version(2, device.driverVersion), vulkan_format_version(3, device.driverVersion), vulkan_format_version(0, device.driverVersion));
		DgLog(DG_LOG_VERBOSE, "\tVendor ID: %.8X", device.vendorID);
		DgLog(DG_LOG_VERBOSE, "\tDevice ID: %.8X", device.deviceID);
		DgLog(DG_LOG_VERBOSE, "\tDevice Type: %s", vulkan_stringify_device_type(device.deviceType));
	}
}

VkPhysicalDevice vulkan_get_best_device_handle(uint32_t count, VkPhysicalDevice *devices) {
	/**
	 * Returns the handle to the most optimal physical device in the system.
	 */
	
	// TODO: Implement properly
	return devices[0];
}

VkResult vulkan_enumerate_queue_family_properties(VkPhysicalDevice device, uint32_t *prop_count, VkQueueFamilyProperties **prop) {
	/**
	 * Enumerate the properties of a queue family for a given device.
	 */
	
	vkGetPhysicalDeviceQueueFamilyProperties(device, prop_count, NULL);
	
	*prop = DgAlloc(sizeof **prop * *prop_count);
	
	if (!*prop) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	vkGetPhysicalDeviceQueueFamilyProperties(device, prop_count, *prop);
	
	return VK_SUCCESS;
}

void vulkan_print_queue_family_properties(uint32_t count, VkQueueFamilyProperties *properties) {
	/**
	 * Print out the queue family properties to the console.
	 */
	
	for (size_t i = 0; i < count; i++) {
		DgLog(DG_LOG_VERBOSE, "Queue family %d:", i);
		DgLog(DG_LOG_VERBOSE, "\tFlags:");
		DgLog(DG_LOG_VERBOSE, "\t\tGraphics: %s", (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No");
		DgLog(DG_LOG_VERBOSE, "\t\tCompute: %s", (properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No");
		DgLog(DG_LOG_VERBOSE, "\t\tTransfer: %s", (properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No");
		DgLog(DG_LOG_VERBOSE, "\t\tSparse binding: %s", (properties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No");
		DgLog(DG_LOG_VERBOSE, "\t\tProtection: %s", (properties[i].queueFlags & VK_QUEUE_PROTECTED_BIT) ? "Yes" : "No");
		DgLog(DG_LOG_VERBOSE, "\tMax queues: %d", properties[i].queueCount);
		DgLog(DG_LOG_VERBOSE, "\tValid timestamp bits: %d", properties[i].timestampValidBits);
		DgLog(DG_LOG_VERBOSE, "\tTransfer granularity: (%d, %d, %d)", properties[i].minImageTransferGranularity.width, properties[i].minImageTransferGranularity.height, properties[i].minImageTransferGranularity.depth);
	}
}

VkResult vulkan_create_logical_device(VkPhysicalDevice physical_device, VkDevice *device, uint32_t queue_count, VkQueueFamilyProperties *queue, uint32_t extension_count, const char ** extension) {
	/**
	 * Creates a logical vulkan device, with all queues available.
	 */
	
	VkDeviceQueueCreateInfo *queue_create_infos = DgAlloc(sizeof *queue_create_infos * queue_count);
	
	if (!queue_create_infos) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	float **priorities = DgAlloc(sizeof *priorities * queue_count);
	
	if (!priorities) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	for (size_t i = 0; i < queue_count; i++) {
		size_t qc = queue[i].queueCount;
		
		priorities[i] = DgAlloc(sizeof **priorities * qc);
		
		if (!priorities[i]) {
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}
		
		for (size_t j = 0; j < qc; j++) {
			priorities[i][j] = 1.0f;
		}
		
		queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		queue_create_infos[i].pNext = NULL;
		queue_create_infos[i].flags = 0;
		queue_create_infos[i].queueFamilyIndex = i;
		queue_create_infos[i].queueCount = queue[i].queueCount;
		queue_create_infos[i].pQueuePriorities = priorities[i];
	}
	
	VkDeviceCreateInfo device_create_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.queueCreateInfoCount = queue_count,
		.pQueueCreateInfos = queue_create_infos,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = extension_count,
		.ppEnabledExtensionNames = extension,
		.pEnabledFeatures = NULL
	};
	
	VkResult status;
	
	if ((status = vkCreateDevice(physical_device, &device_create_info, NULL, device)) != VK_SUCCESS) {
		return status;
	}
	
	for (size_t i = 0; i < queue_count; i++) {
		DgFree(priorities[i]);
	}
	
	DgFree(priorities);
	DgFree(queue_create_infos);
	
	return VK_SUCCESS;
}

void vulkan_free_logical_device(VkDevice device) {
	/**
	 * Destroys a logical vulkan device.
	 */
	
	if (device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(device);
		vkDestroyDevice(device, NULL);
	}
}
