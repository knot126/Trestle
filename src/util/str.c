/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * String Utilities
 */ 

#include <inttypes.h>
#include <string.h>

#include "alloc.h"

char *DgStrcad(const char * const src1, const char * const src2) {
	/**
	 * Concatinate two strings into one, the resulting memory must be freed
	 * manually.
	 */
	
	uint32_t len1 = strlen(src1);
	uint32_t len2 = strlen(src2);
	
	char *ns = DgAlloc((len1 + len2 + 1) * sizeof(char));
	
	memcpy(ns, src1, len1);
	memcpy(ns + len1, src2, len2);
	ns[len1 + len2] = '\0';
	
	return ns;
}

char *DgStrcadf(char *src1, const char * const src2) {
	/*
	 * Same as DgStrcad, but first string is freed after concat. This is useful
	 * for compacting concatenations.
	 */
	char *string = DgStrcad(src1, src2);
	
	DgFree(src1);
	
	return string;
}

char *DgStrdup(char *source) {
	/*
	 * Duplicate a string
	 */
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
