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

DgImageInfo DgLoadImage(char* path);
void DgFreeImage(DgImageInfo* image);
