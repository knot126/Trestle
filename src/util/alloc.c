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

const bool DG_MEMORY_ALLOC_DEBUG = true;

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

static void DgOptimiseMemory() {
	/* 
	 * Goes through the free memory list and combines free spots that are next 
	 * to each other.
	 */
	DgFreeBlockInfo *tnode = dg_alloc.pool_info.next;
	DgFreeBlockInfo *nnode = dg_alloc.pool_info.next->next;
	
	while (nnode != NULL) {
		// If the size of a node and its pointer add to another close node, then
		// they can be merged together
		if (((void *) tnode) + tnode->size == (void *) nnode) {
			if (DG_MEMORY_ALLOC_DEBUG) {
				printf("Optimise memory sections <0x%x> and <0x%x>.\n", tnode, nnode);
			}
			
			tnode->size = tnode->size + nnode->size;
			tnode->next = nnode->next;
			nnode = nnode->next;
		}
		else if (((void *) tnode) + tnode->size > (void *) nnode) {
			printf("Warning: Corrupted data structure in allocator. Values: <0x%x, 0x%x>.\n", (((void *) tnode) + tnode->size), (void *) nnode);
			break;
		}
		else {
			// Cannot be optimised.
			if (DG_MEMORY_ALLOC_DEBUG) {
				printf("Node area cannot be optimised, next...\n");
			}
			
			tnode = nnode;
			nnode = nnode->next;
		}
	}
}

void DgAllocPrintChain() {
	if (!DG_MEMORY_ALLOC_DEBUG) {
		return;
	}
	DgFreeBlockInfo *p = dg_alloc.pool_info.next;
	
	int i = 0;
	
	while (p != NULL) {
		if (p) {
			printf("At node %d: addr = <%X>; size = 0x%X; next = <%X>\n", i, p, p->size, p->next);
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
	
	// Make sure our size is at least the size of a free node
	if (size < sizeof(DgFreeBlockInfo) - sizeof(DgBlockHeader)) {
		size = sizeof(DgFreeBlockInfo) - sizeof(DgBlockHeader);
	}
	
	if (DG_MEMORY_ALLOC_DEBUG) {
		printf("Allocating %d bytes of memory...\n", size);
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
	
	// Return pointer to new block of memory
	return memory;
}

void DgFree(void *block) {
	/* 
	 * Frees a block of memory, assuming this is a valid pointer to memory
	 * that we have allocated. 
	 */
	
	if (DG_MEMORY_ALLOC_DEBUG) {
		printf("Freeing block of memory at <0x%X>...\n", block);
	}
	
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
	
	DgOptimiseMemory();
}

void *DgRealloc(void *block, size_t size) {
	/*
	 * Reallocates a memory pool, resizing if possible or moving if not.
	 */
	
	// Make sure our size is at least the size of a free node
	if (size < sizeof(DgFreeBlockInfo) - sizeof(DgBlockHeader)) {
		size = sizeof(DgFreeBlockInfo) - sizeof(DgBlockHeader);
	}
	
	if (DG_MEMORY_ALLOC_DEBUG) {
		printf("Reallocating block of memory at <0x%X> to size %d...\n", block, size);
	}
	
	// Get the memory info
	DgBlockHeader *meminfo = (DgBlockHeader *) (block - sizeof(DgBlockHeader));
	size_t old_size = meminfo->size;
	
	if (DG_MEMORY_ALLOC_DEBUG) {
		printf("Realloc: was %d bytes, now %d bytes.\n", old_size, size);
	}
	
	if (size > old_size) {
		// Find a well-sized block with a memory address one grather than the ending
		// of the block to resize
		DgFreeBlockInfo *last = NULL;
		DgFreeBlockInfo *have = dg_alloc.pool_info.next;
		
		while (have != NULL) {
			if ((void *) have == ((void *) block) + size && have->size > (size - old_size)) {
				break;
			}
			last = have;
			have = have->next;
		}
		
		// Need to move
		if (!have) {
			if (DG_MEMORY_ALLOC_DEBUG) {
				printf("Realloc: resizing by moving values.\n", last, have);
			}
			
			void *new = DgAlloc(size);
			
			if (!new) {
				printf("Failed to reallocate memory.\n");
				return NULL;
			}
			
			memcpy(new, block, old_size);
			DgFree(block);
			return new;
		}
		
		// Otherwise we make room, and we already know there is enough room
		if (have->size <= sizeof(DgFreeBlockInfo) * 2) {
			// The case where the new part is less than a DgFreeBlockInfo.
			if (DG_MEMORY_ALLOC_DEBUG) {
				printf("Realloc: overtaking block to reallocate.\n", last, have);
			}
			
			last->next = have->next;
			meminfo->size = old_size + have->size;
			return block;
		}
		else {
			// The case where there is enough room to still fit a free block
			// after the new allocation
			if (DG_MEMORY_ALLOC_DEBUG) {
				printf("Realloc: resizing block to reallocate.\n", last, have);
			}
			
			size_t diff = size - old_size;
			void *memory = (void *) meminfo;
			DgFreeBlockInfo *new_free = (DgFreeBlockInfo *) (memory + size + sizeof(DgBlockHeader));
			
			meminfo->size = size;
			new_free->size = have->size - diff;
			new_free->next = have->next;
			last->next = new_free;
			
			return block;
		}
	}
	else if (size < old_size) {
		DgFreeBlockInfo *last = NULL;
		DgFreeBlockInfo *have = dg_alloc.pool_info.next;
		
		while (have != NULL) {
			if ((void *) have > (block + size + sizeof(DgBlockHeader))) {
				break;
			}
			last = have;
			have = have->next;
		}
		
		if (DG_MEMORY_ALLOC_DEBUG) {
			printf("Realloc: inserting between <0x%x> and <0x%x>.\n", last, have);
		}
		
		DgFreeBlockInfo *new_free = (DgFreeBlockInfo *) (block + size + sizeof(DgBlockHeader));
		
		if (last) {
			last->next = new_free;
		}
		else {
			dg_alloc.pool_info.next = new_free;
		}
		new_free->size = size;
		new_free->next = have;
		
		DgOptimiseMemory();
		
		return block;
	}
	else {
		return block;
	}
}
