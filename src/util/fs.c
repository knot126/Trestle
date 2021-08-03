/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * File System Utilites
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#if defined(__linux__)
	#include <sys/stat.h>
	#include <dirent.h>
	#include <errno.h>
#endif

#include "alloc.h"
#include "types.h"
#include "log.h"

#include "fs.h"

const char * const ASSETS_LOOK_PATH[4] = {
	"./assets",
	"../Engine/assets",
	"../Trestle/assets",
	"../assets",
};

const char *dg_special_directory_paths[3] = {
	NULL,
	NULL,
	".",
};

void DgInitPaths(uint32_t fail_mode) {
	/**
	 * This will initialise the paths in dg_special_directory_paths.
	 * 
	 * NOTE: This needs to be redone for not only ZIP files but also just to be
	 * better written in general.
	 */
	
	// Index 0: Assets path
	for (int i = 0; i < sizeof(ASSETS_LOOK_PATH); i++) {
		bool d = DgIsDir(ASSETS_LOOK_PATH[i]);
		
		if (d) {
			dg_special_directory_paths[0] = ASSETS_LOOK_PATH[i];
			break;
		}
	}
	
	if (!dg_special_directory_paths[0]) {
		if (fail_mode == DG_PATH_FAIL_ERROR) {
			printf("Failed to initialise paths!!");
		}
		else {
			printf("Couldn't initialise paths.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Index 1: User config path ** NOT IMPLEMENTED **
	
}

DgFileStream* DgFileStreamOpen(char* path, char* permissions) {
	/* Opens a file stream */
	FILE *f;
	f = fopen(path, permissions);
	if (!f) {
		DgLog(DG_LOG_ERROR, "Failed to open file stream at %s.", path);
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

void DgFileStreamWrite(DgFileStream* stream, size_t size, const void* data) {
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

bool DgFileStreamEndOfFile(DgFileStream *stream) {
	/**
	 * Get if the stream has EOF marked.
	 */
	
	return !!feof(stream->_c_file_stream);
}

char *DgFileStreamGetString(DgFileStream* stream, size_t *length) {
	/**
	 * Get a string from a file stream. Here, a string is terminated by either 
	 * a \0, \n, EOF or \r. This returns NULL on failure or the string on
	 * success.
	 * 
	 * If length is not NULL, then the string size is written to length.
	 * 
	 * NB: The returned string must be freed afterwards.
	 */
	
	int c;
	size_t size = 0;
	size_t alloc = 0;
	char *str = NULL;
	
	if (DgFileStreamEndOfFile(stream)) {
		return NULL;
	}
	
	while (true) {
		if (size >= alloc) {
			alloc = 4 + (alloc * 2);
			
			str = (char *) DgRealloc(str, alloc);
			
			if (!str) {
				return NULL;
			}
		}
		
		c = fgetc(stream->_c_file_stream);
		
		if (c == '\0' || c == '\n' || c == EOF || c == '\r') {
			break;
		}
		
		str[size] = (char) c;
		
		size++;
	}
	
	str = (char *) DgRealloc(str, size + 1);
	
	if (!str) {
		return NULL;
	}
	
	str[size] = '\0';
	
	if (length) {
		*length = size;
	}
	
	return str;
}

/**
 * Non-File Stream Functions
 * -------------------------
 * 
 * Generic file utilities, not for streams!
 */

char* DgEvalPath(char* path) {
	/* 
	 * Expands/evaluates a DGFS path. Remember to free the allocated memory.
	 * 
	 * DGFS DESCRIPTION
	 * ================
	 * 
	 * DGFS allows using a single, standard file path for all platforms. It 
	 * (tries to) abstracts the normal file system without going too far.
	 * 
	 *   * asset://            - Access to the game's main asset bundle.
	 *   * config://           - Access to the user's save directory.
	 *   * bin://              - Access to the binaries directory.
	 */
	
	// Check for a path that does not need evaluation
	if (!strstr(path, "://")) {
		return path;
	}
	
	uint8_t i; // For the path to use
	
	// Note we only check the first two bytes
	if (!memcmp(path, "as", 2)) {
		i = 0;
	}
	else if (!memcmp(path, "co", 2)) {
		i = 1;
	}
	else if (!memcmp(path, "bi", 2)) {
		i = 2;
	}
	else {
		return NULL;
	}
	
	// Here, we account for: old len + added "/" len + special len + null term
	size_t size = strlen(path) + strlen(dg_special_directory_paths[i]) + (sizeof(char) * 2);
	char *new_path = (char *) DgAlloc(size);
	char *rel_path = strstr(path, "://") + (sizeof(char) * 3);
	
	strcpy(new_path, dg_special_directory_paths[i]);
	strcat(new_path, "/");
	strcat(new_path, rel_path);
	
	return new_path;
}

void DgMkdir(char* path) {
	/* Create a new directory */
#if defined(__linux__)
	mkdir(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#else
	DgLog(DG_LOG_WARNING, "Function DgMkdir() is not implemented for this platform.\n");
#endif
}

bool DgIsDir(const char* dir) {
#if defined(__linux__)
	DIR *d = opendir(dir);
	if (d) {
		return true;
	}
#else
	DgLog(DG_LOG_WARNING, "Function DgIsDir is not implemented for this platform.\n");
#endif
	return false;
}

char *DgGetUserDir() {
	return NULL;
}

void DgDeleteFile(char* path) {
	/** 
	 * Delete a file 
	 */
	int status = remove(path);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to remove file %s.", path);
	}
}

void DgMoveFile(char* src, char* dest) {
	/** 
	 * Move a file 
	 */
	int status = rename(src, dest);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to rename file %s.", src);
	}
}

void DgCopyFile(char* src, char* dest) {
	DgLog(DG_LOG_WARNING, "Function DgCopyFile() is not implemented.");
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

inline void DgFileStreamReadLine(DgFileStream* stream, size_t max, char *data) {
	char *c = fgets(data, max, stream->_c_file_stream);
	if (!c) {
		DgLog(DG_LOG_ERROR, "Failed to read line from file stream.");
	}
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

inline void DgFileStreamWriteLine(DgFileStream* stream, char *data) {
	fwrite(data, strlen(data) * sizeof(char), 1, stream->_c_file_stream);
	char n = '\n';
	fwrite(&n, 1, 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteString(DgFileStream* stream, char *data) {
	fwrite(data, strlen(data) * sizeof(char), 1, stream->_c_file_stream);
}
