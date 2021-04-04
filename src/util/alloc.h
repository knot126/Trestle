/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>
#include <inttypes.h>

// NOTE: Should be replaced with DgHandle32
typedef int32_t alloch_t;

// Allocation functions
void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);

// Utility functions bundled with allocator
char *DgStrdup(char *source);
