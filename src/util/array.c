/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Dynamic Array Helper
 */

#include <stdio.h>
#include <inttypes.h>

#include "alloc.h"
#include "fail.h"
#include "types.h"

#include "array.h"

Array trArray(const uint32_t type_size, const uint32_t count) {
	trArray_* array = DgAlloc(sizeof(trArray_));
	
	array->size = (uint32_t) count;
	array->type_size = (uint32_t) type_size;
	array->allocated = array->size * array->type_size;
	array->data = DgAlloc(array->allocated);
	
	if (!array->data) {
		DgFail("Failed to allocate memory for array.", 1);
	}
	
	return array;
}

Array trArrayResize(Array array, const uint32_t count) {
	array->size = count;
	array->allocated = array->size * array->type_size;
	array->data = (byte *) DgRealloc(array->data, array->allocated);
	
	if (!array->data) {
		DgFail("Failed to allocate memory for array.", 1);
	}
	
	return array;
}

void trArrayFree(Array array) {
	DgFree(array->data);
	DgFree(array);
}

byte *trArrayPointer(Array array) {
	return array->data;
}

void trArrayPush(Array array, byte *data) {
	
}

void trArrayPop(Array array, byte *data) {
	
}
