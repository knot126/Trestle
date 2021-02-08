/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * PropertyBag implementation
 */

#include <string.h>
#include <stdlib.h>

#include "alloc.h"
#include "bag.h"

DgPropertyBag DgPropertyBagInit(size_t size) {
	/* Create a new property bag with size elements already initialised. */
	DgPropertyBag pb;
	
	pb.pairs = (DgPropertyBagPair *) DgAlloc(sizeof(DgPropertyBagPair) * size);
	memset(&pb.pairs, 0, sizeof(DgPropertyBagPair) * size);
	
	return pb;
}

char *DgPropertyBagGet(DgPropertyBag pb, char* name) {
	/* Takes a PropertyBag and key and returns the value if found.
	 * 
	 * Returns the value string location.
	 */
	char* value = "";
	
	for (size_t i = 0; i < pb.size; i++) {
		if (pb.pairs[i].key && !strcmp(pb.pairs[i].key, name)) {
			value = pb.pairs[i].value;
			break;
		}
	}
	
	return value;
}

void DgPropertyBagSet(DgPropertyBag pb, char* key, char* value) {
	
}
