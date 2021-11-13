/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan Communication Module
 */

#include <string.h>
#include "misc/grpreq.h"
#include "util/log.h"

#include "core.h"

#define VK_CHECK(EXPR) { \
	VkResult e = EXPR; \
	\
	if (e) {\
		DgLog(DG_LOG_ERROR, "Vulkan: Line %d: status %d", __LINE__, e);\
	}\
}

VulkanSocket *vulkan_init(VulkanSocket *this) {
	/**
	 * Initialise a new vulkan instance
	 */
	
	VkResult status;
	
	memset(this, 0, sizeof *this);
	
	status = vkEnumerateInstanceVersion(&this->version);
	
	if (status != VK_SUCCESS) {
		this->error = status;
		return NULL;
	}
	
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
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = 0,
		.ppEnabledExtensionNames = NULL,
	};
	
	status = vkCreateInstance(&this->instance_create_info, NULL, &this->instance);
	
	if (status != VK_SUCCESS) {
		this->error = status;
		return NULL;
	}
	
	return this;
}

void vulkan_free(VulkanSocket *this) {
	/**
	 * Free a vulkan instance
	 */
}
