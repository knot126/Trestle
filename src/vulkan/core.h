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
} VulkanSocket;

VulkanSocket *vulkan_init(VulkanSocket *this);
void vulkan_free(VulkanSocket *this);
