/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bitmaps and Other Images
 */ 

#include <inttypes.h>

#include "rand.h"
#include "alloc.h"
#include "maths.h"

#include "bitmap.h"

enum {
	DG_BITMAP_DEFAULT_CHANNELS_COUNT = 3, // For some random generation functions
};

uint32_t DgBitmapNew(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan) {
	/**
	 * Initialise a bitmap that has already been allocated. Returns 1 on success
	 * or 0 on failure to allocate memory.
	 */
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = chan;
	bitmap->src = (uint8_t *) DgAlloc(width * height * DG_BITMAP_DEFAULT_CHANNELS_COUNT * sizeof(uint8_t));
	
	if (!bitmap->src) {
		DgFree(bitmap);
		return 0;
	}
	
	uint8_t *src = bitmap->src;
	
	return 1;
}

void DgBitmapFree(DgBitmap *bitmap) {
	/**
	 * Free a bitmap and all of its data.
	 */
	
	if (bitmap->src) {
		DgFree(bitmap->src);
	}
}

DgBitmap *DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgVec4 colour) {
	/**
	 * Draw a single pixel to a bitmap.
	 * 
	 * WARNING: This function's current implementation assumes that the image is
	 * an 8-bit RGBA image.
	 */
	
	this->src[(((y * this->width) + x) * this->chan) + 0] = (uint8_t) (colour.r * 255.0f);
	if (this->chan >= 2) {
		this->src[(((y * this->width) + x) * this->chan) + 1] = (uint8_t) (colour.g * 255.0f);
		if (this->chan >= 3) {
			this->src[(((y * this->width) + x) * this->chan) + 2] = (uint8_t) (colour.b * 255.0f);
			if (this->chan >= 4) {
				this->src[(((y * this->width) + x) * this->chan) + 3] = (uint8_t) (colour.a * 255.0f);
			}
		}
	}
	
	return this;
}

DgBitmap *DgBitmapFill(DgBitmap *this, DgVec4 colour) {
	/**
	 * Fill a bitmap with a given colour.
	 * 
	 * NOTE: See WARNING on DgBitmapDrawPixel.
	 */
	
	for (size_t x = 0; x < this->width; x++) {
		for (size_t y = 0; y < this->height; y++) {
			DgBitmapDrawPixel(this, x, y, colour);
		}
	}
	
	return this;
}

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size) {
	/**
	 * Allocates and generates a bitmap in a tile format. The image will have
	 * three channels (RGB) and be 8-bit.
	 */
	
	DgBitmap *bitmap = (DgBitmap *) DgAlloc(sizeof(DgBitmap));
	
	if (!bitmap) {
		return NULL;
	}
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = DG_BITMAP_DEFAULT_CHANNELS_COUNT;
	bitmap->src = (uint8_t *) DgAlloc(width * height * DG_BITMAP_DEFAULT_CHANNELS_COUNT * sizeof(uint8_t));
	
	if (!bitmap->src) {
		DgFree(bitmap);
		return NULL;
	}
	
	uint8_t *src = bitmap->src;
	
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
		}
		
		if (size_tick_x <= 0) {
			current_x = ~current_x;
			size_tick_x = size;
		}
		
		current_y = current_x;
	}
	
	return bitmap;
}

DgBitmap *DgBitmapRandom(const uint16_t width, const uint16_t height) {
	/**
	 * Generate a random bitmap.
	 */
	
	DgBitmap *bitmap = (DgBitmap *) DgAlloc(sizeof(DgBitmap));
	
	if (!bitmap) {
		return NULL;
	}
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = DG_BITMAP_DEFAULT_CHANNELS_COUNT;
	bitmap->src = (uint8_t *) DgAlloc(width * height * DG_BITMAP_DEFAULT_CHANNELS_COUNT * sizeof(uint8_t));
	
	if (!bitmap->src) {
		DgFree(bitmap);
		return NULL;
	}
	
	uint8_t *src = bitmap->src;
	
	for (uint32_t i = 0; i < (bitmap->width * bitmap->height * bitmap->chan); i++) {
		src[i] = (uint8_t) DgRandInt() % 256;
	}
	
	return bitmap;
}
