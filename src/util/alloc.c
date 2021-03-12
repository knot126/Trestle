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
