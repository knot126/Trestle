// first attempt at a realloc
// it failed very well


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
			if ((void *) have == ((void *) block) + old_size && have->size > (size - old_size)) {
				break;
			}
			last = have;
			have = have->next;
		}
		
		// Need to move
		if (!have) {
			void *new = DgAlloc(size);
			
			if (DG_MEMORY_ALLOC_DEBUG) {
				printf("Realloc: resizing by moving values from <0x%x> to <0x%x>.\n", block, new);
			}
			
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
			if (last) {
				last->next = new_free;
			}
			else {
				dg_alloc.pool_info.next = new_free;
			}
			
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

