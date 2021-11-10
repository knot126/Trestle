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
	
	memset(this, 0, sizeof *this);
	
	return this;
}

void vulkan_free(VulkanSocket *this) {
	/**
	 * Free a vulkan instance
	 */
}
