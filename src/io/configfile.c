/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Config File Parser and Loader
 */

#include <string.h>

#include "../util/bag.h"
#include "../util/alloc.h"
#include "../io/fs.h"

#include "configfile.h"

void DgLoadConfigFileToBag(DgBag* pb, char* path) {
	path = DgEvalPath(path);
	
	DgLoadBinaryFileInfo *file = DgLoadBinaryFile(path);
	
	// Free path name string
	DgFree(path);
	
	
}
