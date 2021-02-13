/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * PropertyBag implementation
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "alloc.h"

#include "bag.h"

DgBag DgBagInit() {
	/* Create a new property bag with size elements already initialised. */
	DgBag pb;
	
	pb.key = (const char **) DgAlloc(sizeof(const char *));
	
	if (!pb.key) {
		printf("Failed to allocate memory for bag keys.\n");
		return pb;
	}
	
	pb.value = (const char **) DgAlloc(sizeof(const char *));
	
	if (!pb.value) {
		printf("Failed to allocate memory for bag values.\n");
		return pb;
	}
	
	memset(pb.key, 0, sizeof(const char *));
	
	pb.size = 1;
	
	return pb;
}

void DgBagFree() {
	
}

const char *DgBagGet(DgBag pb, const char* key) {
	/* 
	 * Takes a PropertyBag and key and returns the value if found.
	 * Returns the value string location.
	 */
	char* value = "";
	
	for (size_t i = 0; i < pb.size; i++) {
		if (pb.key[i] && !strcmp(pb.key[i], key)) {
			pb.value[i] = value;
			break;
		}
	}
	
	return value;
}

void DgBagSet(DgBag pb, const char* key, const char* value) {
	/* 
	 * Adds (or reassigns the key if found) the key by the name key to the value
	 * value.
	 */
	bool have = false;
	
	for (size_t i = 0; i < pb.size; i++) {
		if (pb.key[i] && !strcmp(pb.key[i], key)) {
			have = true;
			pb.value[i] = value;
			break;
		}
	}
	
	if (!have) {
		pb.size++;
		
		pb.key = (const char **) DgRealloc(pb.key, sizeof(const char *) * pb.size);
		pb.value = (const char **) DgRealloc(pb.value, sizeof(const char *) * pb.size);
		
		if (!pb.key || !pb.value) {
			printf("Failed to allocate memory to make new pair in bag.\n");
			return;
		}
		
		pb.key[pb.size - 1] = key;
		pb.value[pb.size - 1] = value;
	}
}
