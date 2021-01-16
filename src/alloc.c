#include <stdlib.h>

#include "alloc.h"

static const int _AllocChunkSize = 2048;
static DgPoolInfo _AllocPools[30];

void *DgMakePool(size_t size) {
	/* Requests a memory pool from the system. */
	void *p = malloc(size * _AllocChunkSize);
	
	return p;
}

void DgFreePool(void* p) {
	/* Frees a memory pool. */
	free(p);
}

void *DgAlloc(/*DgAllocMode mode,*/ size_t size) {
	return 0;
}
