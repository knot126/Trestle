/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Dictionary - Hopefully the last implementation!
 */

#include <string.h>

#include "alloc.h"

#include "dict.h"

DgDict DgDictNew(void) {
	DgDict dict;
	
	memset(&dict, 0, sizeof(DgDict));
	
	return dict;
}

DgDict *DgDictNew2(void) {
	DgDict *dict = (DgDict *) DgAlloc(sizeof(DgDict));
	
	if (dict) {
		memset(dict, 0, sizeof(DgDict));
	}
	
	return dict;
}

void DgDictSet(DgDict *dict, DgDictItem *key, DgDictItem *value) {
	/*
	 * Set an item in a dictionary
	 */
	for (size_t i = 0; i < dict->length; i++) {
		if (dict->data[i].key.type == key->type && dict->data[i].key.data.asPtr == key->data.asPtr) {
			DgDictItem *k = &dict->data[i].key;
			
			if (k->type == DG_DICT_STRING || k->type == DG_DICT_ARRAY || k->type == DG_DICT_DICT) {
				DgFree(k->data.asPtr);
			}
			
			dict->data[i].value = *value;
			
			return;
		}
	}
	
	dict->data = (DgDictPair *) DgRealloc(dict->data, sizeof(DgDictPair) * ++dict->length);
	dict->data[dict->length - 1].key = *key;
	dict->data[dict->length - 1].value = *value;
}

DgDictItem* DgDictGet(DgDict *dict, DgDictItem *key) {
	/*
	 * Get an item from a dictionary
	 */
	for (size_t i = 0; i < dict->length; i++) {
		if (dict->data[i].key.type == key->type && dict->data[i].key.data.asPtr == key->data.asPtr) {
			return &dict->data[i].value;
		}
	}
}

