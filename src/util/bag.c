/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * PropertyBag implementation
 * DEPRECATED: do not use this anymore!!
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#warning DgBag is deprecated.

#include "alloc.h"

#include "bag.h"

DgBag DgBagInit() {
	/* 
	 * Create a new property bag with one element already initialised.
	 */
	DgBag pb;
	
	pb.key = (const char **) 0;
	pb.value = (const char **) 0;
	pb.size = 0;
	
	return pb;
}

void DgBagFree(DgBag* pb) {
	/*
	 * Free a property bag 
	 */
}

void DgBagPrint(DgBag* pb) {
	/*
	 * Print the contents of a bag 
	 */
	if (!pb->key || !pb->value) {
		printf("\t<property bag is null>\n");
		return;
	}
	
	printf("{\n");
	for (size_t i = 0; i < pb->size; i++) {
		if (pb->key[i] && pb->value[i]) {
			printf("\t\"%s\": \"%s\",\n", pb->key[i], pb->value[i]);
		}
		else {
			printf("\t<key or value is null>\n");
		}
	}
	printf("}\n");
}

const char *DgBagGet(DgBag* pb, const char* key) {
	/* 
	 * Takes a PropertyBag and key and returns the value if found.
	 * Returns the value string location.
	 */
	const char *value = "";
	
	if (!pb->key || !pb->value) {
		return value;
	}
	
	for (size_t i = 0; i < pb->size; i++) {
		if (pb->key[i] && !strcmp(pb->key[i], key)) {
			value = pb->value[i];
			break;
		}
	}
	
	return value;
}

void DgBagSet(DgBag* pb, const char* key, const char* value) {
	/* 
	 * Adds (or reassigns the key if found) the key by the name key to the value
	 * value.
	 */
	bool have = false;
	
	for (size_t i = 0; i < pb->size; i++) {
		if (pb->key[i] && !strcmp(pb->key[i], key)) {
			have = true;
			pb->value[i] = value;
			break;
		}
	}
	
	if (!have) {
		pb->size++;
		
		pb->key = (const char **) DgRealloc(pb->key, sizeof(const char *) * pb->size);
		pb->value = (const char **) DgRealloc(pb->value, sizeof(const char *) * pb->size);
		
		if (!pb->key || !pb->value) {
			printf("Failed to allocate memory to make new pair in bag.\n");
			return;
		}
		
		pb->key[pb->size - 1] = key;
		pb->value[pb->size - 1] = value;
	}
}
