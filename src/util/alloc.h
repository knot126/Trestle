/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>
#include <inttypes.h>

typedef struct DgPoolInfo {
	void* memory;
	void* next;
	size_t size;
} DgPoolInfo;

typedef struct DgBlockInfo {
	size_t size; // TODO: Is this really needed?
	void* next;
} DgBlockInfo;

typedef struct DgFreeBlockInfo {
	size_t size;
	void* block;
	struct DgFreeBlockInfo *next;
} DgFreeBlockInfo;

typedef struct DgPoolInfo2 {
	// Information about the main block
	void* block;
	size_t block_size;
	
	// The head, points to first free space
	DgFreeBlockInfo* next;
} DgPoolInfo2;

typedef struct DgBlockHeader {
	size_t size;
} DgBlockHeader;

typedef int32_t alloch_t;

int32_t DgMakePool(size_t size);
void DgFreePool(int32_t index);
int32_t DgBestPoolIndex(size_t size);
void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);
