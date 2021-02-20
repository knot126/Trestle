/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#include "../util/alloc.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(sz) DgAlloc(sz)
#define STBI_REALLOC(p, newsz) DgRealloc(p, newsz)
#define STBI_FREE(p) DgFree(p)
#include "../../lib/stb/stb_image.h"

#include "../io/fs.h"
#include "../types.h"

#include "image.h"

DgImageInfo DgLoadImage(char* path) {
	/* Load an image into memory */
	DgImageInfo info;
	
	path = DgEvalPath(path);
	
	info.data = (byte *) stbi_load(path, &info.width, &info.height, &info.channels, 0);
	
	DgFree(path); // Free pathname
	
	return info;
}
