#include <inttypes.h>

typedef struct DgPoolInfo {
	void* memory;
	void* next;
	size_t size;
	// size_t free;
} DgPoolInfo;

typedef struct DgBlockInfo {
	size_t size; // TODO: Is this really needed?
	void* next;
} DgBlockInfo;

typedef int32_t alloch_t;

int32_t DgMakePool(size_t size);
void DgFreePool(int32_t index);
int32_t DgBestPoolIndex(size_t size);
void *DgAlloc(size_t size);
void DgFree(void *block);
