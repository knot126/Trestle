/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Dynamic Array Helper
 */

#pragma once

#include <inttypes.h>

typedef struct DgArray {
	void *data;
	uint32_t size;
	uint32_t type_size;
	uint32_t allocated;
	uint8_t __extra_space__[4];
} DgArray;

DgArray DgArrayNew(const uint32_t type_size, const uint32_t count);
void DgArrayResize(DgArray *array, const uint32_t count);
