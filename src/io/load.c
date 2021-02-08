#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "../util/alloc.h"
#include "../types.h"
#include "fs.h"

#include "load.h"

DgLoadBinaryFileInfo DgLoadBinaryFile(char* path) {
	/* 
	 * Loads a binary file into memory and returns information about the loaded
	 * file.
	 */
	DgLoadBinaryFileInfo info;
	DgFileStream* file;
	byte* content;
	size_t length;
	
	file = DgFileStreamOpen(path, "rb");
	
	if (!file) {
		exit(-1);
	}
	
	length = DgFileStreamLength(file);
	
	content = (byte *) DgAlloc((sizeof(char) * length));
	
	if (!content) {
		printf("Failed to allocate memory for loading binfile.\n");
	}
	
	DgFileStreamRead(file, length, content);
	
	DgFileStreamClose(file);
	
	info.data = content;
	info.size = length;
	
	return info;
}
