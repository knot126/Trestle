/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#include <inttypes.h>
#include <stdint.h>

#include "util/alloc.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(s) DgAlloc(s)
#define STBI_REALLOC(p, s) DgRealloc(p, s)
#define STBI_FREE(p) DgFree(p)
#include "extern/stb_image.h"

#include "util/fs.h"
#include "util/types.h"
#include "util/rand.h"
#include "util/log.h"

#include "image.h"

enum {
	DG_IMAGE_CHANNELS = 3,
};

DgImageInfo DgLoadImage(char* path) {
	/* Load an image into memory */
	DgImageInfo info;
	
	char *res_path = DgEvalPath(path);
	
	// TODO: Figure out how to undo this but keep font textures okay
	stbi_set_flip_vertically_on_load(1);
	
	info.data = (byte *) stbi_load(res_path, &info.width, &info.height, &info.channels, 0);
	
	if (!info.data) {
		DgLog(DG_LOG_ERROR, "DgLoadImage: Loaded image at '%s'.", path);
	}
	
	DgFree(res_path); // Free pathname
	
	return info;
}

void DgFreeImage(DgImageInfo* image) {
	stbi_image_free(image->data);
	image->data = 0;
}
