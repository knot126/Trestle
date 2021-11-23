/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GPU Compute and Graphics
 */ 

#pragma once

#include <inttypes.h>

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
	VkDevice device;
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
};

typedef int_fast16_t ComputeStatus;

ComputeStatus compute_init(ComputeSystem * restrict this);
void compute_free(ComputeSystem * restrict this);
