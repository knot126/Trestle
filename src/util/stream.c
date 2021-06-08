/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Generic Stream Utilites
 */

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "alloc.h"
#include "log.h"

#include "stream.h"

_Static_assert(sizeof(uint8_t) == 1);

static void DgStreamInit(DgStream *stream, void *buffer, size_t prealloc) {
	/**
	 * Initialises a stream based on the contents of buffer and prealloc.
	 * 
	 * If buffer is NULL, then the size of data in prealloc is preallocated for
	 * the buffer. If buffer is a pointer to some valid memory, then buffer is 
	 * the memory that the stream will be set to use, and prealloc is the size
	 * of that buffer.
	 * 
	 * When creating from an existing buffer, the head is set to the last byte
	 * in the size; otherwise, it is set to zero.
	 */
	
	if (!buffer) {
		stream->data = (uint8_t *) DgAlloc(prealloc);
		
		if (!stream->data) {
			stream->error = DG_STRM_ALLOC_ERROR;
			return;
		}
	}
	else {
		stream->data = (uint8_t *) buffer;
	}
	
	stream->allocated = prealloc;
	stream->size = (!!buffer) ? prealloc : 0;
	stream->head = (!!buffer) ? prealloc - 1 : 0;
}

DgStream *DgStreamCreate(void) {
	/**
	 * Creates a stream in memory from no exsiting stream.
	 */
	
	DgStream *stream = (DgStream *) DgAlloc(sizeof(DgStream));
	
	if (!stream) {
		return NULL;
	}
	
	DgStreamInit(stream, NULL, 1024);
	
	return stream;
}

DgStream *DgStreamFromBuffer(void *buffer, size_t size) {
	/**
	 * Creates a stream based on the memory buffer buffer and its size. The
	 * memory will now be managed by the stream.
	 */
	
	DgStream *stream = (DgStream *) DgAlloc(sizeof(DgStream));
	
	if (!stream) {
		return NULL;
	}
	
	DgStreamInit(stream, buffer, size);
	
	return stream;
}

void DgStreamFree(DgStream *stream) {
	/**
	 * Frees all memory allocated by a stream.
	 */
	
	if (stream->data) {
		DgFree(stream->data);
	}
	
	DgFree(stream);
}

void DgBufferFromStream(DgStream *stream, void **pointer, size_t *size) {
	/**
	 * Degrade the stream to a pointer to memory and size. This will free the
	 * memory allocated for the management of the stream.
	 */
	
	*pointer = (void *) stream->data;
	*size = stream->size;
	
	DgFree(stream);
}

size_t DgStreamError(DgStream *stream) {
	/**
	 * Get the latest error from the stream and set the error to DG_STRM_OKAY.
	 */
	
	size_t r = stream->error;
	stream->error = DG_STRM_OKAY;
	return r;
}

size_t DgStreamGetpos(DgStream *stream) {
	/**
	 * Get the current position in the file stream as the offset to the start of
	 * the stream.
	 * 
	 * This will not return the size of the stream if at DG_STRM_END; instead,
	 * it will return (size - 1). If you really need to get the size, use the
	 * respective function for getting the stream data's length.
	 */
	
	return stream->head;
}

size_t DgStreamLength(DgStream *stream) {
	/**
	 * Get the length of the stream.
	 */
	
	return stream->size;
}

void DgStreamSetpos(DgStream *stream, DgStreamEnum offset, int64_t pos) {
	/**
	 * Set the stream position according to the offset. Note that pos is signed
	 * so it can be negitive.
	 */
	
	size_t base;
	
	switch (offset) {
		case DG_STRM_CUR:
			base = stream->head;
			break;
		
		case DG_STRM_SET:
			base = 0;
			break;
		
		case DG_STRM_END:
			base = stream->size - 1;
			break;
		
		default:
			stream->error = DG_STRM_INVALID;
			return;
			break;
	}
	
	base += pos;
	
	if (base < 0 || base >= stream->size) {
		stream->error = DG_STRM_RANGE;
		return;
	}
	
	stream->head = base;
}

void DgStreamRewind(DgStream *stream) {
	/**
	 * Rewind a stream to its starting position, clearing any erorrs.
	 */
	
	stream->head = 0;
	stream->error = DG_STRM_OKAY;
}

void DgStreamRead(DgStream *stream, size_t size, void *buffer) {
	/**
	 * Read a stream's data starting at its current head position until either
	 * the size (provided as size) bytes are read or until the end of the stream
	 * is encountred.
	 * 
	 * The DG_STRM_RANGE error is produced if one of the following happens:
	 * 
	 *   * If the stream ends and the size is still not zero, then the error is
	 *     set. The available bytes will not be copied and other stream state 
	 *     will remain as-is.
	 *   * If the size is zero or buffer is a null pointer, then the error is set.
	 */
	
	// Check that the requested size does not exceede the size of the stream, 
	// and that size > 0 and buffer is not NULL. If any of these are so, then
	// we will need to set error and return.
	if (((stream->head + size) >= stream->size) || (size == 0) || (buffer == NULL)) {
		stream->error = DG_STRM_RANGE;
		return;
	}
	
	// Copy data to the buffer
	memcpy(buffer, (void *) stream->data + size, size);
	
	// Advance the head by size
	stream->head += size;
}

void DgStreamWrite(DgStream *stream, size_t size, void *buffer) {
	/**
	 * Write to a stream at the current head location of the stream. If this
	 * raises any errors, the data should be assumed to be corrupt.
	 */
	
	// Reallocate memory while there is not enough to fit new data
	while ((stream->head + 1 + size) > stream->allocated) {
		stream->allocated *= 2;
		stream->data = (uint8_t *) DgRealloc(stream->data, stream->allocated);
		
		if (!stream->data) {
			stream->error = DG_STRM_ALLOC_ERROR;
			return;
		}
	}
	
	// If writing the data makes the new stream size exceede the current size,
	// then set the stream size to the amount it should be.
	if ((stream->head + 1 + size) > stream->size) {
		stream->size = (stream->head + 1) + size;
	}
	
	// Copy the memory
	memcpy(stream->data + stream->head, buffer, size);
}
