/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#pragma once

#include <inttypes.h>

typedef struct DgImageInfo {
	byte *data;
	int width;
	int height;
	int channels;
} DgImageInfo;

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t chan;
	unsigned char *src;
} DgBitmap;

DgImageInfo DgLoadImage(char* path);
void DgFreeImage(DgImageInfo* image);

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size);
void DgBitmapFree(DgBitmap *bitmap);
