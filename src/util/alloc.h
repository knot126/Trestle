#include <inttypes.h>

/*
typedef enum {
	// Use modes
	DG_ALLOC_PREPARE = 0x00,
	DG_ALLOC_ALLOC = 0x01,
	
	// Allocator modes
	DG_ALLOC_POOL = 0x10,
	DG_ALLOC_LINEAR = 0x11,
	DG_ALLOC_FREELIST = 0x12,
	DG_ALLOC_STACK = 0x13
} DgAllocMode;
*/

typedef struct {
	void* memory;
	uint32_t size;
	uint32_t properties;
} DgPoolInfo;

void *DgAlloc(/*DgAllocMode mode,*/ size_t size);
