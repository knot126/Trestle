/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>
#include <inttypes.h>

// Allocation functions
void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);

// Utility functions bundled with allocator
char *DgStrdup(char *source);
char *DgStrdupl(const char * const source, size_t max);
