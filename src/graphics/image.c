/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#include <stdio.h>

#include "../util/alloc.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(sz) DgAlloc(sz)
#define STBI_REALLOC(p, newsz) DgRealloc(p, newsz)
#define STBI_FREE(p) DgFree(p)
#include "../../lib/stb/stb_image.h"

#include "../util/fs.h"
#include "../util/types.h"

#include "image.h"

DgImageInfo DgLoadImage(char* path) {
	/* Load an image into memory */
	DgImageInfo info;
	
	char *res_path = DgEvalPath(path);
	
	info.data = (byte *) stbi_load(res_path, &info.width, &info.height, &info.channels, 0);
	
	if (info.data) {
		printf("Info: ImageModule: Loaded image at '%s'.\n", path);
	}
	
	DgFree(res_path); // Free pathname
	
	return info;
}

void DgFreeImage(DgImageInfo* image) {
	stbi_image_free(image->data);
	image->data = 0;
}
