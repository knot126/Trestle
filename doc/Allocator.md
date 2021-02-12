# Allocator

*This document mainly describes the new allocator, which is tree free-list based, not the currently implemented one. For the currently implemented allocator, see the Frame Allocator section.*

The main alloctor in the Decent Games Engine is based on the tree free-list model, and uses the functions named `DgAlloc()` and `DgFree()`, which have the same API has `malloc()` and `free()`, respectively. This alloctor can be used by including `"util/alloc.h"`.

The engine also provides other memory allocators: `DgFrameAlloc` (linear, forgotten every frame), `DgConstAlloc` (linear, never deleted), `DgFixedSizeBytePool` (pool allocator with fixed size) and `DgStackAlloc` (stack-based allocator). These should be used sparsingly compared to the general purpose `DgAlloc`.

**Note**: Not all memory allocators are properly implemented at this time.

## Comparason

This list does not cover attributes of each type of allocator in the first place; those can be found elsewhere.

| Allocator name      | Allocator type      | Freeable | Size | Header file       |
| ------------------- | ------------------- | -------- | ---- | ----------------- |
| `DgAlloc`           | Free-list (linked)  | Yes      | Any  | `util/alloc.h`    |
| `DgFrameAlloc`      | Linear (forgotten)  | At Once  | Any  | `util/falloc.h`   |
| `DgConstAlloc`      | Linear              | No       | Any  | `util/calloc.h`   |
| `DgPoolAlloc`       | Pool                | Yes      | Same | `util/palloc.h`   |
| `DgStackAlloc`      | Stack               | Yes (1)  | Any  | `util/salloc.h`   |

 1. All frees must be in reverse order of the allocations.

## API

### `DgAlloc`

```c
void *DgAlloc(size_t size);
```

Allocates a block of memory of at least the size `size`. It could allocate more memory, especially for very small allocations, but this amount is *not defined* because it depends on the size of pointers in the current enviorment.

 * `size` (`size_t`): The minimum size of the memory block to be allocated.
 * **Returns (`void *`)**: An untyped (void) pointer to the start of the memory block that has been allocated.

### `DgFree`

```c
void DgFree(void* block);
```

Frees a pool of memory. The pointer to this memory should not be used after this operation since the pointer is no longer valid. 

 * `block` (`void *`): The pointer to the block that should be removed.

### `DgFrameAllocInit`

```c
void DgFrameAllocInit(size_t size);
```

Initialises the frame allocator and grabs the initial memory block, of size `size`. This should be called once during startup.

 * `size` (`size_t`): The size of the memory block to allocate. This will be the most the frame allocator can support.

### `DgFrameAllocReset`

```c
void DgFrameAllocReset();
```

Resets the frame allocator and forgets all allocations made with it. Should be called once per frame.

### `DgFrameAlloc`

```c
void *DgFrameAlloc(size_t size);
```

Allocates some memory, of exactly `size` size, and returns a pointer to the memory block.

 * `size` (`size_t`): The exact size of the memory block to be allocated.
 * **Returns (`void *`)**: An untyped (void) pointer to the start of the memory block that has been allocated.

## Example

### `DgAlloc` and `DgFree`

```c
#include "util/alloc.h"

void allocExample() {
	const size_t size = 256 * sizeof(char);
	
	// Allocate the memory
	char *myString = (char *) DgAlloc(size);
	
	// Free the memory
	DgFree(myString);
}
```

### `DgFrameAlloc`

```c
#include "math/vector3.h"
#include "util/falloc.h"

void fallocExample() {
	const size_t max_size = 1024; // 1 KiB
	const size_t size = 4 * sizeof(DgVec3);
	
	// Initialise frame alloctor
	DgFrameAllocInit(max_size);
	
	while (mainLoop) {
		// Allocate two four vector arrays
		DgVec3 *myString = (DgVec3 *) DgFrameAlloc(size);
		DgVec3 *myOtherString = (DgVec3 *) DgFrameAlloc(size);
		
		// Reset the allocations
		DgFrameAllocReset();
	}
}
```
