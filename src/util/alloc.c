#include <stdlib.h>
#include <stdio.h>

#include "alloc.h"

static uint16_t dg_alloc_pools_count;
static uint16_t dg_alloc_active_pools_count;
static DgPoolInfo *dg_alloc_pools;

uint16_t DgMakePool(size_t size) {
	/* Requests a memory pool from the system. */
	dg_alloc_pools_count++;
	dg_alloc_active_pools_count++;
	
	if (!dg_alloc_pools) {
		dg_alloc_pools = malloc(sizeof(DgPoolInfo) * dg_alloc_pools_count);
		
		if (!dg_alloc_pools) {
			printf("Failed to allocate alloc pools memory.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Because of this, I think this alloc actually preform worse :)
	// Should just make enough space in the list in the first place.
	dg_alloc_pools = realloc(dg_alloc_pools, sizeof(DgPoolInfo) * dg_alloc_pools_count);
	
	if (!dg_alloc_pools) {
		printf("Warning: Realloc array of pools has failed!\n");
	}
	
	void *pool = malloc(size);
	
	if (!pool) {
		printf("Failed to allocate %d bytes of memory.\n", (uint64_t)size);
		return 0;
	}
	
	dg_alloc_pools[dg_alloc_pools_count - 1].memory = pool;
	dg_alloc_pools[dg_alloc_pools_count - 1].next = pool;
	dg_alloc_pools[dg_alloc_pools_count - 1].size = size;
	
	// Always remember to subtract one from this number! (Of course, this has 
	// been done for you here...)
	return dg_alloc_pools_count - 1;
}

void DgFreePool(uint16_t index) {
	/* Frees a memory pool, and the pools info array if there are no pools are
	 * left. */
	dg_alloc_active_pools_count--;
	free(dg_alloc_pools[index].memory);
	
	dg_alloc_pools[index].memory = NULL;
	dg_alloc_pools[index].next = NULL;
	
	if (dg_alloc_active_pools_count == 0) {
		free(dg_alloc_pools);
	}
}

uint16_t DgBestPoolIndex(size_t size) {
	/* Returns best pool's index */
	uint32_t i;
	
	for (i = 0; i < dg_alloc_pools_count; i++) {
		if (dg_alloc_pools[i].size - (uint32_t)(dg_alloc_pools[i].next - dg_alloc_pools[i].memory) > size) {
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
