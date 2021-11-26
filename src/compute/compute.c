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

static ComputeStatus compute_init_device(ComputeDevice * restrict this, VkPhysicalDevice handle, ComputeSystem * restrict sys) {
	/**
	 * Initialise a compute device, given its handle. If the ComputeSystem is
	 * not NULL, then the extensions will be taken from it.
	 */
	
	memset(this, 0, sizeof *this);
	
	this->physical_device = handle;
	
	if (vulkan_enumerate_queue_family_properties(handle, &this->queue_info_count, &this->queue_info) != VK_SUCCESS) {
		DgLog(DG_LOG_ERROR, "Failed to enumerate queue family properties.");
		return TR_COMPUTE_FAILURE;
	}
	
	vulkan_print_queue_family_properties(this->queue_info_count, this->queue_info);
	
	// Create used counts array
	{
		this->used_counts = DgAlloc(sizeof *this->used_counts * this->queue_info_count);
		
		if (!this->used_counts) {
			DgLog(DG_LOG_ERROR, "Failed to allocate memory for ComputeDevice::used_counts.");
			return TR_COMPUTE_FAILURE;
		}
		
		memset(this->used_counts, 0, sizeof *this->used_counts * this->queue_info_count);
	}
	
	if (vulkan_create_logical_device(
		this->physical_device,
		&this->device,
		this->queue_info_count,
		this->queue_info,
		(sys) ? sys->extension_count : 0,
		(sys) ? sys->extension : NULL) != VK_SUCCESS
	)
	{
		DgLog(DG_LOG_ERROR, "Failed to create logical vulkan device.");
		return TR_COMPUTE_FAILURE;
	}
	
	return TR_COMPUTE_SUCCESS;
}

static ComputeStatus compute_free_device(ComputeDevice * restrict this) {
	/**
	 * Free a compute device.
	 */
	
	DgFree(this->queue_info);
	DgFree(this->used_counts);
	
	vulkan_free_logical_device(this->device);
	
	return TR_COMPUTE_SUCCESS;
}

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
	
	// Set up the best device
	if (compute_init_device(&this->main_device, vulkan_get_best_device_handle(physical_device_count, physical_device), NULL) != TR_COMPUTE_SUCCESS) {
		return TR_COMPUTE_FAILURE;
	}
	
	DgFree(physical_device);
	
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
	
	compute_free_device(&this->main_device);
	
	vkDestroyInstance(this->instance, NULL);
}

