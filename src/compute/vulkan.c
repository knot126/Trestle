/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan Communication Module
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
// Utility functions
// =============================================================================

static const char ** vulkan_copy_string_array(uint32_t length, const char **array) {
	/**
	 * Make a copy of a string array.
	 */
	
	if (!array || !length) {
		return NULL;
	}
	
	const char ** new_array = DgAlloc(sizeof *new_array * length);
	
	if (!new_array) {
		return NULL;
	}
	
	for (uint32_t i = 0; i < length; i++) {
		new_array[i] = DgStrdup(array[i]);
		
		if (!new_array[i]) {
			// Clean up so memory is not leaked
			for (uint32_t j = 0; j < i; j++) {
				DgFree((void *) new_array[j]);
			}
			
			DgFree(new_array);
			return NULL;
		}
	}
	
	return new_array;
}

static void vulkan_free_string_array(uint32_t length, const char **array) {
	/**
	 * Free a given string array fully.
	 */
	
	if (!array) {
		return;
	}
	
	for (uint32_t i = 0; i < length; i++) {
		DgFree((void *) array[i]);
	}
	
	DgFree(array);
}

// =============================================================================
// Instance
// =============================================================================

static VkResult vulkan_instance_init(VulkanSocket *this, uint32_t extension_count, const char **extensions, uint32_t layer_count, const char **layers) {
	/**
	 * Initialise a new vulkan instance
	 */
	
	VkResult status;
	
	// Clear memory
	memset(this, 0, sizeof *this);
	
	// Copy layer and extension arrays
	this->layer = vulkan_copy_string_array(this->layer_count = layer_count, layers);
	this->extension = vulkan_copy_string_array(this->extension_count = extension_count, extensions);
	
	// Get instance version
	status = vkEnumerateInstanceVersion(&this->version);
	
	if (status != VK_SUCCESS) {
		return status;
	}
	
	// Create the vulkan instance
	this->application_info = (VkApplicationInfo) {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = "",
		.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
		.pEngineName = "",
		.engineVersion = VK_MAKE_VERSION(0, 0, 0),
		.apiVersion = VK_MAKE_VERSION(1, 1, 0),
	};
	
	this->instance_create_info = (VkInstanceCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = &this->application_info,
		.enabledLayerCount = this->layer_count,
		.ppEnabledLayerNames = this->layer,
		.enabledExtensionCount = this->extension_count,
		.ppEnabledExtensionNames = this->extension,
	};
	
	status = vkCreateInstance(&this->instance_create_info, NULL, &this->instance);
	
	if (status != VK_SUCCESS) {
		return status;
	}
	
	return VK_SUCCESS;
}

static void vulkan_instance_free(VulkanSocket *this) {
	/**
	 * Free a vulkan instance
	 */
	
	vulkan_free_string_array(this->extension_count, this->extension);
	vulkan_free_string_array(this->layer_count, this->layer);
	
	vkDestroyInstance(this->instance, NULL);
}

// =============================================================================
// Physical and virtual devices
// =============================================================================

static VkResult vulkan_enumerate_devices(VulkanSocket *this) {
	/**
	 * Retrive a list of physical devices in the system.
	 */
	
	vkEnumeratePhysicalDevices(this->instance, &this->physical_device_count, NULL);
	
	this->physical_device = DgAlloc(sizeof *this->physical_device * this->physical_device_count);
	
	if (!this->physical_device) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	vkEnumeratePhysicalDevices(this->instance, &this->physical_device_count, this->physical_device);
}

static VkResult vulkan_devices_init(VulkanSocket *this) {
	/**
	 * Initialise vulkan state related to devices.
	 */
	
	vulkan_enumerate_devices(this);
}

static void vulkan_devices_free(VulkanSocket *this) {
	/**
	 * Free any state related to devices.
	 */
}

// =============================================================================
// Full init and free
// =============================================================================

VkResult vulkan_init(VulkanSocket *this, uint32_t extension_count, const char **extensions, uint32_t layer_count, const char **layers) {
	/**
	 * Initialise a connection to the Vulkan API
	 */
	
	VkResult status;
	
	status = vulkan_instance_init(this, extension_count, extensions, layer_count, layers);
	
	if (status != VK_SUCCESS) {
		return status;
	}
	
	return VK_SUCCESS;
}

VkResult vulkan_init_simple(VulkanSocket *this) {
	/**
	 * Initialise a vulkan socket without asking for the details to use when
	 * cerating that socket.
	 */
	
	vulkan_init(this, 0, NULL, 0, NULL);
}

void vulkan_free(VulkanSocket *this) {
	/**
	 * Break a connection to the vulkan API
	 */
	
	vulkan_instance_free(this);
}
