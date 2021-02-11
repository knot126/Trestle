#pragma once

#include "../types.h"
#include "fs.h"

typedef struct DgLoadBinaryFileInfo {
	const byte* data;
	size_t size;
} DgLoadBinaryFileInfo;

DgLoadBinaryFileInfo *DgLoadBinaryFile(char* path);
void DgUnloadBinaryFile(DgLoadBinaryFileInfo *);
