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

void *DgAlloc(size_t size);
