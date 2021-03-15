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

// Init and destroy main allocator
int32_t DgAllocPoolInit(size_t size);
void DgAllocPoolFree(int32_t handle);

// Allocation functions
void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);

// Debugging functions
void DgAllocPrintChain();

/* 
 * Linear Allocation
 */

typedef struct DgLinearAllocPoolInfo {
	char name[16];
	size_t size;
	void * memory;
	void * next;
} DgLinearAllocPoolInfo;
