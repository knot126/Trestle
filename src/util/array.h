/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Dynamic Array Helper
 */

#pragma once

#include <inttypes.h>

#include "../types.h"

typedef struct trArray_ {
	byte *data;
	uint32_t size;
	uint32_t type_size;
	uint32_t allocated;
	uint8_t __extra_space__[4];
} trArray_;

typedef trArray_* Array;

Array trArray(const uint32_t type_size, const uint32_t count);
Array trArrayResize(Array array, const uint32_t count);
void trArrayFree(Array array);
byte *trArrayPointer(Array array);

void trArrayPush(Array array, byte *data);
void trArrayPop(Array array, byte *data);
