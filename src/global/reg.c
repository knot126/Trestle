/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Registry for global settings
 */

// required includes
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include "util/alloc.h"
#include "util/str.h"
#include "util/hash.h"

#include "reg.h"

void registry_init(Registry * const restrict t) {
	/**
	 * Initialise the hash table at the given address.
	 */
	
	memset(t, 0, sizeof *t);
}

static int8_t RegistryRealloc(Registry * const restrict t) {
	/**
	 * Reallocate the hash table if it needs to be reallocated. This should only
	 * be used by internal functions.
	 */
	
	if (t->alloc == t->count) {
		t->alloc = 2 + (t->alloc * 2);
		
		t->key = DgRealloc(t->key, sizeof *t->key * t->alloc);
		if (!t->key) { return 1; }
		
		t->hash = DgRealloc(t->hash, sizeof *t->hash * t->alloc);
		if (!t->hash) { return 2; }
		
		t->value = DgRealloc(t->value, sizeof *t->value * t->alloc);
		if (!t->value) { return 3; }
	}
	
	return 0;
}

int8_t registry_set(Registry * const restrict t, const char * k, char * v) {
	/**
	 * Set a key to the given value.
	 */
	
	if (RegistryRealloc(t)) {
		return 1;
	}
	
	size_t index = 0;
	uint32_t want = DgHashStringU32(k);
	char found = 0;
	
	// find item
	for (size_t i = 0; i < t->count; ++i) {
		if (t->hash[i] == want) {
			if (!strcmp(t->key[i], k)) {
				index = i;
				found = 1;
				break;
			}
		}
	}
	
	// Add to the end of the list if it was not found
	if (!found) {
		index = t->count;
		t->count++;
	}
	
	t->key[index] = DgStrdup(k);
	t->hash[index] = want;
	t->value[index] = DgStrdup(v);
	
	return 0;
}

char *registry_get(Registry * const restrict t, const char * k) {
	/**
	 * Get a reference to an object in the hash table.
	 */
	
	size_t index = 0;
	uint32_t want = DgHashStringU32(k);
	char found = 0;
	
	// find item
	for (size_t i = 0; i < t->count; ++i) {
		if (t->hash[i] == want) {
			if (!strcmp(t->key[i], k)) {
				index = i;
				found = 1;
				break;
			}
		}
	}
	
	// replace the current key, value and hash without reallocating if found
	if (found) {
		return t->value[index];
	}
	
	return NULL;
}

void registry_free(Registry * const restrict t) {
	/**
	 * Free the entire hash table.
	 */
	
	if (t->key) {
		// Free the keys
		for (size_t i = 0; i < t->count; i++) {
			(void) DgFree((void *) t->key[i]);
		}
		
		t->key = DgRealloc(t->key, 0);
	}
	
	if (t->hash) {
		t->hash = DgRealloc(t->hash, 0);
	}
	
	if (t->value) {
		// Free the values
		for (size_t i = 0; i < t->count; i++) {
			(void) DgFree(t->value[i]);
		}
		
		t->value = DgRealloc(t->value, 0);
	}
	
	t->alloc = 0;
	t->count = 0;
}
