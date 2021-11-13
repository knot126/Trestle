/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan Communication Module
 */

#pragma once

#include "misc/grpreq.h"

typedef struct VulkanSocket {
	VkInstance instance;
	VkResult error;
	uint32_t version;
	VkInstanceCreateInfo instance_create_info;
	VkApplicationInfo application_info;
} VulkanSocket;

VulkanSocket *vulkan_init(VulkanSocket *this);
void vulkan_free(VulkanSocket *this);
