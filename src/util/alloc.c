/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 * 
 * NOTE: This has just been passed to malloc for now because the previous 
 * implementation was pretty bad. It will be rewritten soon!
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../io/fs.h"

#include "alloc.h"

int32_t DgAllocPoolInit(size_t size) {
	// needs to be reimplemented
	printf("Note: DgAllocPoolInit(%d) called.\n", size);
	return 0;
}

void DgAllocPoolFree(int32_t handle) {
	// needs to be reimplemented
	printf("Note: DgAllocPoolFree(%d) called.\n", handle);
}

void DgAllocPrintChain() {
	// needs to be reimplemented
	printf("Note: DgAllocPrintChain() called.\n");
}

void *DgAlloc(size_t size) {
	return malloc(size);
}

void DgFree(void *block) {
	free(block);
}

void *DgRealloc(void* block, size_t size) {
	return realloc(block, size);
}

/* 
 * Linear Allocation
 */

// TODO: This should use dynamic memory allocation.
DgLinearAllocPoolInfo _dg_linear_alloc_pool_info[20];
bool _dg_linear_alloc_pool_info_allocated[20];

bool DgLAllocNewPool(const char * name, const size_t size) {
	int8_t index = -1;
	
	for (int8_t i = 0; i < 20; i++) {
		if (!_dg_linear_alloc_pool_info_allocated[i]) {
			index = i;
			break;
		}
	}
	
	if (index == -1) {
		return false;
	}
	
	DgLinearAllocPoolInfo * pool = &_dg_linear_alloc_pool_info[index];
	
	pool->size = size;
	// Note: does not check len becuase there should be no reason to do that.
	strcpy(pool->name, name);
	
	pool->memory = DgAlloc(size);
	
	if (!pool->memory) {
		return false;
	}
	
	pool->next = pool->memory;
	
	return true;
}

void DgLAllocFreePool(const char * name) {
	
}
