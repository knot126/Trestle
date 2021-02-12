/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Memory Allocation
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "alloc.h"

// DEPRECATED: Try to avoid using these variables
static uint32_t dg_alloc_pools_count;
static uint32_t dg_alloc_active_pools_count;

// Pointer to pool information, the size of the array of pools and an array of 
// if the pools are free or not
// TODO: It may also be worthwhile to consider storing the freeness with each 
// pool, but believe that would take more memory.
DgPoolInfo  *dg_alloc_pools;
DgPoolInfo2 *dg_alloc_pools2;
size_t       dg_alloc_pools_size;
bool        *dg_alloc_pools_frees; // false = free, true = allocated

int32_t DgMakePool(size_t size) {
	/* 
	 * Requests a new memory pool from the system.
	 * 
	 * Returns a handle (index) of the memory pool information. Positive means
	 * a successful run, negitive number means an error occured.
	 */
	
	// This will be done the first time that pools are allocated
	if (!dg_alloc_pools) {
		// Pre-allocate for 16 new pools
		dg_alloc_pools = malloc(sizeof(DgPoolInfo) * 16);
		dg_alloc_pools_frees = calloc(sizeof(bool) * 16, 1);
		dg_alloc_pools_size = 16;
		
		if (!dg_alloc_pools || !dg_alloc_pools_frees) {
			printf("Failed to allocate alloc pools memory.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Find the next free pool
	int32_t got = -1;
	
	for (int32_t i = 0; got < 0 && i < dg_alloc_pools_size; i++) {
		if (dg_alloc_pools_frees[i] == false) {
			got = i;
		}
	}
	
	// Reallocate if no pool slots are free
	if (got == -1) {
		dg_alloc_pools_size += 8;
		dg_alloc_pools = realloc(dg_alloc_pools, sizeof(DgPoolInfo) * dg_alloc_pools_size);
		dg_alloc_pools_frees = realloc(dg_alloc_pools_frees, sizeof(bool) * dg_alloc_pools_size);
		
		// NOTE: What this should do is set the last eight (new) bools in the
		// array to zero, so we know they are all set to false and there will be
		// one free.
		memset(dg_alloc_pools_frees + (dg_alloc_pools_size * sizeof(bool) - 8),
			   0,
			   dg_alloc_pools_size);
		
		if (!dg_alloc_pools || !dg_alloc_pools_frees) {
			printf("__FILE__:__LINE__ System alloc failed!\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Before you swear: Remember this is the pool alloc function, NOT the
	// actual alloc function.
	void *pool = malloc(size);
	
	if (!pool) {
		printf("Warning: Failed to allocate %d bytes of memory.\n", (uint64_t) size);
		return -1;
	}
	
	dg_alloc_pools[got].memory = pool;
	dg_alloc_pools[got].next = pool;
	dg_alloc_pools[got].size = size;
	dg_alloc_pools_frees[got] = true;
	
	return got;
}

void DgFreePool(int32_t index) {
	/* Frees a memory pool, and the pools info array if there are no pools are
	 * left. */
	
	// Refuse to free a failed pool
	if (index < 0) {
		printf("Warning: Tried to free a failed pool.\n");
		return;
	}
	
	free(dg_alloc_pools[index].memory);
	
	dg_alloc_pools[index].memory = NULL;
	dg_alloc_pools[index].next = NULL;
	dg_alloc_pools[index].size = 0;
	
	dg_alloc_pools_frees[index] = false;
}

int32_t DgBestPoolIndex(size_t size) {
	/* Returns best pool's index.
	 *
	 * Currently, best pool is defined as the first with enough space to hold
	 * the data.
	 */
	int32_t i;
	
	for (i = 0; i < dg_alloc_pools_size; i++) {
		if (
			dg_alloc_pools_frees[i] == true // Check that the pool is allocated
			&& dg_alloc_pools[i].size 
				- (int32_t)(dg_alloc_pools[i].next - dg_alloc_pools[i].memory) 
					> size
		) {
			break;
		}
	}
	
	return i;
}

void *DgAlloc(size_t size) {
	/* Allocate a block of memory in the first best fitting pool. */
	DgPoolInfo* pool = &dg_alloc_pools[DgBestPoolIndex(size)];
	
	void *block = pool->next;
	pool->next = pool->next + size + sizeof(DgBlockInfo);
	
	// Check that enough memory is available
	if (pool->next >= pool->memory + pool->size) {
		printf("Not enough memory left in pool allocate %d bytes.\n", (uint64_t)size);
		pool->next = block;
		return NULL;
	}
	
	// Set up block info
	DgBlockInfo *info = block;
	info->size = size;
	info->next = pool->next;
	
	// Move the block pointer to account for the header
	block = block + sizeof(DgBlockInfo);
	
	return block;
}

void DgFree(void *block) {
	
}

void *DgRealloc(void *block, size_t size) {
	printf("DgRealloc is not implemented. Exiting so nothing bad happens.\n");
	exit(-1);
}
