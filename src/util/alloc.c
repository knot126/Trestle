/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 * 
 * NOTE: This has just been passed to malloc for now because the previous 
 * implementation was pretty bad. It will be rewritten soon!
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "alloc.h"

void *DgAlloc(size_t size) {
	return malloc(size);
}

void DgFree(void *block) {
	free(block);
}

void *DgRealloc(void* block, size_t size) {
	return realloc(block, size);
}

char *DgStrdup(char *source) {
	char *dest = DgAlloc(strlen(source) + 1);
	return strcpy(dest, source);
}

char *DgStrdupl(const char * const source, size_t max) {
	/*
	 * Duplicate a string up to a certian length
	 */
	size_t len = 0;
	
	// Get size
	while (len < max && source[len] != '\0') {
		len++;
	}
	
	// Allocate memory
	char *dest = DgAlloc(len + 1);
	
	if (!dest) {
		return NULL;
	}
	
	// Copy bytes
	for (size_t i = 0; i < len; i++) {
		dest[i] = source[i];
	}
	
	// Set last to null terminator
	dest[len] = '\0';
	
	// Return new string
	return dest;
}
