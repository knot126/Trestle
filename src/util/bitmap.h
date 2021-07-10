/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bitmaps and Other Images
 */  

#pragma once

#include <inttypes.h>

#include "maths.h"

typedef struct {
	union {
		uint8_t *src;
		uint16_t *src_16bit;
		float *src_float;
	};
	uint16_t width;
	uint16_t height;
	uint16_t chan;
} DgBitmap;

DgBitmap *DgBitmapCreate(const uint16_t width, const uint16_t height, const uint16_t chan);
DgBitmap *DgBitmapFill(DgBitmap *this, DgVec4 colour);
DgBitmap *DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgVec4 colour);

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size);
DgBitmap *DgBitmapRandom(const uint16_t width, const uint16_t height);
void DgBitmapFree(DgBitmap *bitmap);
