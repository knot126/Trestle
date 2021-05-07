/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>
#include <inttypes.h>

void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);
