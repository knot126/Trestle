/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GPU Compute and Graphics
 */ 

#pragma once

#include <inttypes.h>

/**
 * Compute Device
 * ==============
 * 
 * A structure representing a device that can do compute work, along with its 
 * properties.
 */
typedef struct ComputeDevice {
	// Physical Device
	VkPhysicalDevice physical_device;
	
	// Queue info
	uint32_t queue_info_count;
	VkQueueFamilyProperties *queue_info;
	uint32_t *used_counts;
	
	// Logical device
	VkDevice device;
} ComputeDevice;

/**
 * Compute System
 * ==============
 * 
 * A system that tracks most compute (as in on any computing device) scheduling
 * and work.
 */
typedef struct ComputeSystem {
	// Instance
	VkInstance instance;
// 	uint32_t version;
	const char **layer;
	uint32_t layer_count;
	const char **extension;
	uint32_t extension_count;
	VkInstanceCreateInfo instinfo;
	VkApplicationInfo appinfo;
	
	// Devices
	ComputeDevice main_device;
} ComputeSystem;

/**
 * Compute Status
 * ==============
 * 
 * Result code of any compute command.
 */
enum {
	TR_COMPUTE_SUCCESS = 0x0000,
	TR_COMPUTE_AWAIT = 0x0001,
	TR_COMPUTE_INCOMPLETE = 0x0002,
	TR_COMPUTE_OUT_OF_NEAR_MEMORY = -0x0001,
	TR_COMPUTE_OUT_OF_FAR_MEMORY = -0x0002,
	TR_COMPUTE_FAILURE = -0x0003,
	TR_COMPUTE_INVALID = -0x0004,
};

typedef int_fast16_t ComputeStatus;

/**
 * Compute Mode
 * ============
 * 
 * Specifies the mode to use for compute.
 */
enum {
	TR_COMPUTE_MODE_NONE = 0x0000,
	TR_COMPUTE_MODE_GRAPHICS = 0x0001,
	TR_COMPUTE_MODE_COMPUTE = 0x0002,
};

typedef int_fast16_t ComputeMode;

ComputeStatus compute_init(ComputeSystem * restrict this);
void compute_free(ComputeSystem * restrict this);
