#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#if defined(__linux__)
	#include <sys/stat.h>
#endif

#include "../util/alloc.h"
#include "../types.h"

#include "fs.h"

char* DgEvalPath(char* path) {
	/* 
	 * Expands/evaluates a DGFS path
	 * 
	 * DGFS DESCRIPTION
	 * ================
	 * 
	 * DGFS allows using a single, standard file path for all platforms. It 
	 * (tries to) abstracts the normal file system without going too far.
	 * 
	 *   * asset://            - Access to the game's main asset folder.
	 *   * data://             - Access to the user's save directory.
	 *   * user://             - Access to the user's home directory.
	 *   * logs://             - Access to the program logs directory.
	 *   * bin://              - Access to the binaries directory.
	 *   * <any relivate path> - Prepends the asset directory.
	 */
}

DgFileStream* DgFileStreamOpen(char* path, char* permissions) {
	/* Opens a file stream */
	FILE *f;
	f = fopen(path, permissions);
	if (!f) {
		printf("Failed to open file stream at %s.\n", path);
		return 0;
	}
	DgFileStream *s = (DgFileStream *) DgAlloc(sizeof(DgFileStream));
	s->_c_file_stream = f;
	return s;
}

DgFileStream* DgFileStreamReopen(DgFileStream* stream, char* path, char* permissions) {
	
}

DgFileStream* DgChangeStreamPermissions(DgFileStream* stream, char* permissions) {
	
}

void DgFileStreamClose(DgFileStream* stream) {
	/* Frees a file stream */
	fclose(stream->_c_file_stream);
	DgFree(stream);
}

void DgFileStreamRead(DgFileStream* stream, size_t size, void* data) {
	/*
	 * Read size bytes from stream into data
	 */
	fread(data, sizeof(byte), size, stream->_c_file_stream);
}

void DgFileStreamWrite(DgFileStream* stream, size_t size, void* data) {
	/*
	 * Write size bytes from data into stream
	 */
	fwrite(data, sizeof(byte), size, stream->_c_file_stream);
}

size_t DgFileStreamLength(DgFileStream* stream) {
	/*
	 * Get the length of the file in stream
	 */
	size_t size;
	
	// printf("<0x%X>\n", stream->_c_file_stream);
	
	fseek(stream->_c_file_stream, 0, SEEK_END);
	size = ftell(stream->_c_file_stream);
	rewind(stream->_c_file_stream);
	
	return size;
}

void DgMkdir(char* path) {
	/* Create a new directory */
#if defined(__linux__)
	mkdir(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#else
	printf("Warning: Function DgMkdir() is not implemented for this platform.\n");
#endif
}

void DgDeleteFile(char* path) {
	/* Delete a file */
	int status = remove(path);
	
	if (status) {
		printf("Failed to remove file %s.", path);
	}
}

void DgMoveFile(char* src, char* dest) {
	/* Move a file */
	int status = rename(src, dest);
	
	if (status) {
		printf("Failed to rename file %s.", src);
	}
}

void DgCopyFile(char* src, char* dest) {
	
}

/* =============================================================================
 * Datatype reading functions 
 */

inline void DgFileStreamReadInt8(DgFileStream* stream, int8_t* data) {
	fread(data, sizeof(int8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt8(DgFileStream* stream, uint8_t* data) {
	fread(data, sizeof(uint8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadInt16(DgFileStream* stream, int16_t* data) {
	fread(data, sizeof(int16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt16(DgFileStream* stream, uint16_t* data) {
	fread(data, sizeof(uint16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadInt32(DgFileStream* stream, int32_t* data) {
	fread(data, sizeof(int32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt32(DgFileStream* stream, uint32_t* data) {
	fread(data, sizeof(uint32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadInt64(DgFileStream* stream, int64_t* data) {
	fread(data, sizeof(int64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt64(DgFileStream* stream, uint64_t* data) {
	fread(data, sizeof(uint64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadFloat(DgFileStream* stream, float* data) {
	fread(data, sizeof(float), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadDouble(DgFileStream* stream, double* data) {
	fread(data, sizeof(double), 1, stream->_c_file_stream);
}

/* =============================================================================
 * Datatype writing functions 
 */

inline void DgFileStreamWriteInt8(DgFileStream* stream, int8_t* data) {
	fwrite(data, sizeof(int8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt8(DgFileStream* stream, uint8_t* data) {
	fwrite(data, sizeof(uint8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteInt16(DgFileStream* stream, int16_t* data) {
	fwrite(data, sizeof(int16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt16(DgFileStream* stream, uint16_t* data) {
	fwrite(data, sizeof(uint16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteInt32(DgFileStream* stream, int32_t* data) {
	fwrite(data, sizeof(int32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt32(DgFileStream* stream, uint32_t* data) {
	fwrite(data, sizeof(uint32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteInt64(DgFileStream* stream, int64_t* data) {
	fwrite(data, sizeof(int64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt64(DgFileStream* stream, uint64_t* data) {
	fwrite(data, sizeof(uint64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteFloat(DgFileStream* stream, float* data) {
	fwrite(data, sizeof(float), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteDouble(DgFileStream* stream, double* data) {
	fwrite(data, sizeof(double), 1, stream->_c_file_stream);
}
