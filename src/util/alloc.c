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

#include "alloc.h"

void *DgAlloc(size_t size) {
	return malloc(size);
}

void DgFree(void *block) {
	free(block);
}

void *DgRealloc(void* block, size_t size) {
	return realloc(block, size);
}
