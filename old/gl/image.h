/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#pragma once

#include <inttypes.h>

typedef struct DgImageInfo {
	uint8_t *data;
	int width;
	int height;
	int channels;
} DgImageInfo;

DgImageInfo DgLoadImage(char* path);
void DgFreeImage(DgImageInfo* image);
