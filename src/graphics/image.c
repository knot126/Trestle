/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#include <stdio.h>
#include <inttypes.h>

#include "../util/alloc.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(s) DgAlloc(s)
#define STBI_REALLOC(p, s) DgRealloc(p, s)
#define STBI_FREE(p) DgFree(p)
#include "../../lib/stb/stb_image.h"

#include "../util/fs.h"
#include "../util/types.h"

#include "image.h"

enum {
	DG_IMAGE_CHANNELS = 3,
};

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

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size) {
	DgBitmap *bitmap = (DgBitmap *) DgAlloc(sizeof(DgBitmap));
	
	if (!bitmap) {
		return NULL;
	}
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = DG_IMAGE_CHANNELS;
	bitmap->src = (unsigned char *) DgAlloc(width * height * DG_IMAGE_CHANNELS * sizeof(unsigned char));
	
	if (!bitmap->src) {
		DgFree(bitmap);
		return NULL;
	}
	
	unsigned char *src = bitmap->src;
	
	uint16_t size_tick_x = size;
	uint16_t size_tick_y = size;
	uint8_t current_x = 0;
	uint8_t current_y = 0;
	
	for (uint16_t x = 0; x < width; x++, size_tick_x--) {
		for (uint16_t y = 0; y < height; y++, size_tick_y--) {
			src[(x * width * 3) + (y * 3) + 0] = current_y;
			src[(x * width * 3) + (y * 3) + 1] = current_y;
			src[(x * width * 3) + (y * 3) + 2] = current_y;
			
			if (size_tick_y <= 0) {
				current_y = ~current_y;
				size_tick_y = size;
			}
			printf("X %d | %d | %d\n", current_y, size_tick_y, y);
		}
		
		if (size_tick_x <= 0) {
			current_x = ~current_x;
			size_tick_x = size;
		}
		
		current_y = current_x;
		
		printf("Z %d | %d\n", current_x, x);
	}
	
	return bitmap;
}

void DgBitmapFree(DgBitmap *bitmap) {
	if (bitmap->src) {
		DgFree(bitmap->src);
	}
	
	DgFree(bitmap);
}
