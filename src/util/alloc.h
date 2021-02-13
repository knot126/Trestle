/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>
#include <inttypes.h>

typedef struct DgFreeBlockInfo {
	size_t size;
	void* block;
	struct DgFreeBlockInfo *next;
} DgFreeBlockInfo;

typedef struct DgPoolInfo {
	// Information about the main block
	void* block;
	size_t block_size;
	
	// The head, points to first free space
	DgFreeBlockInfo* next;
} DgPoolInfo;

typedef struct DgBlockHeader {
	size_t size;
} DgBlockHeader;

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
