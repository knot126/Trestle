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
