/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GPU Compute and Graphics
 */ 

#include "util/alloc.h"
#include "util/log.h"
#include "misc/grpreq.h"
#include "compute/vulkan.h"

#include "compute.h"

static ComputeStatus compute_init_vulkan(ComputeSystem * restrict this) {
	/**
	 * Initialise the vulkan instance.
	 */
	
	// Initialise the instance of vulkan
	this->extension = glfwGetRequiredInstanceExtensions(&this->extension_count);
	
	if (!this->extension) {
		DgLog(DG_LOG_ERROR, "glfwGetRequiredInstanceExtensions: returned NULL.");
		return TR_COMPUTE_FAILURE;
	}
	
	for (size_t i = 0; i < this->extension_count; i++) {
		DgLog(DG_LOG_VERBOSE, "Require vulkan extension: %s", this->extension[i]);
	}
	
	if (vulkan_instance_init(
			&this->instance,
			NULL,
			&this->appinfo,
			&this->instinfo,
			this->extension_count,
			this->extension,
			this->layer_count,
			this->layer) != VK_SUCCESS)
	{
		return TR_COMPUTE_FAILURE;
	}
	
	// Get the physical device to use
	VkPhysicalDevice *physical_device;
	uint32_t physical_device_count;
	
	if (vulkan_enumerate_devices(
			this->instance,
			&physical_device_count,
			&physical_device
		) != VK_SUCCESS || physical_device_count < 1)
	{
		return TR_COMPUTE_FAILURE;
	}
	
	vulkan_print_device_properties(physical_device_count, physical_device);
	
	return TR_COMPUTE_SUCCESS;
}

ComputeStatus compute_init(ComputeSystem * restrict this) {
	/**
	 * Initialise the compute system and any subsystems.
	 */
	
	memset(this, 0, sizeof *this);
	
	ComputeStatus status = TR_COMPUTE_SUCCESS;
	
	if (status = compute_init_vulkan(this)) {
		DgLog(DG_LOG_ERROR, "compute_init_vulkan failed with errorcode %d", status);
	}
	
	return status;
}

void compute_free(ComputeSystem * restrict this) {
	/**
	 * Release the compute system and any subsystems that only it operates with.
	 */
	
	vkDestroyInstance(this->instance);
}

