/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vulkan Socket - Higher level connection to Vulkan API
 */

#include "util/alloc.h"
#include "util/str.h"

#include "vksock.h"

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
