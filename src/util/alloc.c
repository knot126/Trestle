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

struct {
	DgPoolInfo pool_info;
} dg_alloc;

int32_t DgAllocPoolInit(size_t size) {
	/* Initialises a memory pool, adding to the list is if there is not one. */
	
	if (dg_alloc.pool_info.block) {
		printf("Memory pool already initialised.");
		return -1;
	}
	
	dg_alloc.pool_info.block = malloc(size);
	
	if (!dg_alloc.pool_info.block) {
		printf("Failed to allocate memory pool of size %d bytes.\n", size);
		return -1;
	}
	
	dg_alloc.pool_info.block_size = size;
	dg_alloc.pool_info.next = (DgFreeBlockInfo *) dg_alloc.pool_info.block;
	
	dg_alloc.pool_info.next->size = size;
	dg_alloc.pool_info.next->next = NULL;
	
	return 0;
}

void DgAllocPoolFree(int32_t handle) {
	/* Frees a memory pool assocatied with the allocator given the handle. */
	if (handle > -1) {
		free(dg_alloc.pool_info.block);
	}
}

static void DgAllocPrintChain() {
	DgFreeBlockInfo *p = dg_alloc.pool_info.next;
	
	int i = 0;
	
	while (p != NULL) {
		if (p) {
			printf("At node %d: addr = <%X>; size = %d; next = <%X>\n", i, p, p->size, p->next);
			i++;
			p = p->next;
		}
	}
}

void *DgAlloc(size_t size) {
	/* 
	 * Allocates some memory
	 * This function assumes that the memory alloctor has been set up properly.
	 */
	
	printf("Allocating %d bytes of memory...\n", size);
	
	DgAllocPrintChain();
	
	// Make sure our size is at least the size of a free node
	if (size < sizeof(DgFreeBlockInfo) - sizeof(DgBlockHeader)) {
		size = sizeof(DgFreeBlockInfo) - sizeof(DgBlockHeader);
	}
	
	// Find the next chunk that will fit
	DgFreeBlockInfo *lnode = NULL;
	DgFreeBlockInfo *cnode = dg_alloc.pool_info.next;
	
	while (cnode != NULL) {
		if (cnode->size >= size) {
			break;
		}
		lnode = cnode;
		cnode = cnode->next;
	}
	
	if (!cnode) {
		printf("DgAlloc: Failed to allocate %d bytes of memory.\n", size);
		return NULL;
	}
	
	// The pointer to cnode is where the new memory will be.
	void* memory = (void *) cnode;
	
	// Handle the case where there is not enough memory to store a new node
	// by just making this allocation aligned to handle for it.
	if (sizeof(DgFreeBlockInfo) > cnode->size - size) {
		DgFreeBlockInfo *nnode = cnode->next;
		
		size = cnode->size - sizeof(DgBlockHeader);
		if (lnode) {
			lnode->next = nnode;
		}
		else {
			dg_alloc.pool_info.next = nnode;
		}
	}
	// Else if there is enough room, handle this normally and move the node up
	// to account for the new allocation.
	else {
		// Create a node according to the size of the memory to be allocated
		DgFreeBlockInfo *tnode = (DgFreeBlockInfo *) (memory + size + sizeof(DgBlockHeader));
		
		// Copy next attribute, calulate and store node size
		tnode->next = cnode->next;
		tnode->size = cnode->size - (size + sizeof(DgBlockHeader));
		
		// Update last node
		if (lnode) {
			lnode->next = tnode;
		}
		else {
			dg_alloc.pool_info.next = tnode;
		}
	}
	
	// Now we actually prepare the memory block!
	DgBlockHeader *block_info = (DgBlockHeader *) memory;
	memory = memory + sizeof(DgBlockHeader);
	block_info->size = size;
	
	DgAllocPrintChain();
	
	// Return pointer to new block of memory
	return memory;
}

void DgFree(void *block) {
	/* 
	 * Frees a block of memory, assuming this is a valid pointer to memory
	 * that we have allocated. 
	 */
	
	printf("Freeing block of memory at <0x%X>...\n", block);
	
	DgAllocPrintChain();
	
	// Get back to memory address and its size
	void *memory = (void *) (block - sizeof(DgBlockHeader));
	size_t size = ((DgBlockHeader *) memory)->size + sizeof(DgBlockHeader);
	
	// Find first node that is greater and last node that is less in memory 
	// address than the area where we want to free is.
	DgFreeBlockInfo *lnode = NULL;
	DgFreeBlockInfo *nnode = dg_alloc.pool_info.next;
	
	while (nnode != NULL) {
		// We don't use nnode->next here; we would need to increment one more time
		// since lnode and nnode would still be less and only the next is actually
		// higher.
		if ((void *) nnode > memory) {
			break;
		}
		lnode = nnode;
		nnode = nnode->next;
	}
	
	// Make node and insert; remember that if this is a valid alloc, we will
	// be okay in terms of space.
	DgFreeBlockInfo *tnode = (DgFreeBlockInfo *) memory;
	
	tnode->size = size;  // Set size
	
	// Are we last?
	if (nnode) {
		// If NOT:
		tnode->next = nnode;
	}
	else {
		// If SO:
		tnode->next = NULL;
	}
	
	// Are we first?
	if (lnode) {
		// If NOT:
		lnode->next = tnode;
	}
	else {
		// If SO:
		dg_alloc.pool_info.next = tnode;
	}
	
	DgAllocPrintChain();
	
	// Now we need to optimise the list
	
	tnode = dg_alloc.pool_info.next;
	nnode = dg_alloc.pool_info.next->next;
	
	while (nnode != NULL) {
		if ((tnode + tnode->size) == nnode) {
			// Merge these nodes
			tnode->size = tnode->size + nnode->size;
// 			printf("tnode->size : %d\n", tnode->size);
			tnode->next = nnode->next;
// 			printf("tnode->next : %d\n", tnode->next);
// 			printf("nnode (before) : <%X>\n", nnode);
			nnode = nnode->next;
// 			printf("nnode : <%X>\n", nnode);
// 			printf("tnode : <%X>\n", tnode);
		}
		else if ((tnode + tnode->size) > nnode) {
			// This should never be true.
			printf("Warning: Corrupted data structure in allocator. Not continuing optimisation operation. Values: <%X, %X>.\n", (tnode + tnode->size), nnode);
			break;
		}
		else {
			printf("Node area cannot be optimised, next...\n");
			tnode = nnode;
			nnode = nnode->next;
		}
	}
	
}

void *DgRealloc(void *block, size_t size) {
	return NULL;
}
