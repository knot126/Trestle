#include <stdio.h>
#include <string.h>

#include "fs.h"
#include "../util/alloc.h"

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

DgFileStream* DgOpenFileStream(char* path, char* permissions) {
	/* Opens a file stream */
	FILE *f;
	f = fopen(path, permissions);
	DgFileStream *s = (DgFileStream *) DgAlloc(sizeof(DgFileStream));
	s->_c_file_stream = f;
	return s;
}

DgFileStream* DgReopenFileStream(DgFileStream* stream, char* path, char* permissions) {
	
}

DgFileStream* DgChangeStreamPermissions(DgFileStream* stream, char* permissions) {
	
}

void DgFreeFileStream(DgFileStream* stream) {
	/* Frees a file stream */
	fclose(stream->_c_file_stream);
	DgFree(stream);
}

void DgMkdir(char* path) {
	
}

void DgDeleteFileStream(DgFileStream* stream) {
	DgFreeFileStream(stream);
}

void DgDeleteFile(char* path) {
	
}

DgFileStream* DgMoveFileStream(DgFileStream* stream, char* path) {
	
}

void DgMoveFile(char* src, char* dest) {
	
}

DgFileStream* DgCopyFileStream(DgFileStream* stream, char* path) {
	
}

void DgCopyFile(char* src, char* dest) {
	
}
