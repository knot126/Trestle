/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bitmaps and Other Images
 */  

#pragma once

#include <inttypes.h>

typedef struct {
	uint8_t *src;
	uint16_t width;
	uint16_t height;
	uint16_t chan;
} DgBitmap;

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size);
DgBitmap *DgBitmapRandom(const uint16_t width, const uint16_t height);
void DgBitmapFree(DgBitmap *bitmap);

