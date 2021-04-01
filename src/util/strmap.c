/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Simple String Map (aka Dictionary) helper
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "../util/alloc.h"

#include "strmap.h"

inline static uint32_t strflen(char *str) {
	uint32_t len;
	
	while (*str != '\0') {
		len++;
	}
	
	return len + 1;
}

char **DgStrmapSet(const char **map, const char *key, const char *value) {
	/*
	 * Set a value 
	 */
	
	// Try finding the value first
	
}
