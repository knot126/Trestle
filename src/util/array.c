/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Dynamic Array Helper
 */

#include <stdio.h>
#include <inttypes.h>

#include "../util/alloc.h"
#include "../util/fail.h"

#include "array.h"

DgArray DgArrayNew(const uint32_t type_size, const uint32_t count) {
	DgArray array;
	
	array.size = (uint32_t) count;
	array.type_size = (uint32_t) type_size;
	array.allocated = array.size * array.type_size;
	array.data = DgAlloc(array.allocated);
	
	if (!array.data) {
		DgFail("Failed to allocate memory for array.", 1);
	}
	
	return array;
}

void DgArrayResize(DgArray *array, const uint32_t count) {
	array->size = count;
	array->allocated = array->size * array->type_size;
	array->data = DgRealloc(array->data, array->allocated);
	
	if (!array->data) {
		DgFail("Failed to allocate memory for array.", 1);
	}
}

void DgArrayFree(DgArray *array) {
	DgFree(array->data);
}
