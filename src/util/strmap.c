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

#include "alloc.h"

#include "strmap.h"

inline static uint32_t strflen(const char *str) {
	uint32_t len;
	
	while (*str != '\0') {
		len++;
	}
	
	return len + 1;
}

inline static char *strfcpy(const char *str) {
	uint32_t len = strflen(str);
	char *nstr = (char *) DgAlloc(len);
	
	memcpy(nstr, str, len);
	
	return nstr;
}

uint32_t DgStrmapSize(char * const * const map) {
	uint32_t size = 0;
	
	for (uint32_t i = 0; map[i]; i += 2) {
		size++;
	}
	
	return size;
}

char **DgStrmapSet(char **map, const char * const key, const char * const value) {
	/*
	 * Set a value 
	 */
	
	// Try finding the value first
	for (uint32_t i = 0; map[i]; i += 2) {
		if (!strcmp(map[i], key)) {
			// Found the value, free old and copy in the new one.
			DgFree(map[i + 1]);
			map[i + 1] = strfcpy(value); 
			return map;
		}
	}
	
	// The string was not found, need to reallocate
	uint32_t size = DgStrmapSize(map) + 1;
	
	map = (char **) DgRealloc(map, (size * sizeof(char *) * 2) + sizeof(char *));
	
	map[(size * 2) - 2] = strfcpy(key);
	map[(size * 2) - 1] = strfcpy(value);
	map[(size * 2)] = NULL;
	
	return map;
}

const char *DgStrmapGet(const char * const * const map, const char * const key) {
	const char *value = NULL;
	
	for (uint32_t i = 0; map[i]; i += 2) {
		if (!strcmp(map[i], key)) {
			value = map[i + 1];
			break;
		}
	}
	
	return value;
}
