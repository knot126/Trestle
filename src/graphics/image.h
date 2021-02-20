/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#pragma once

typedef struct DgImageInfo {
	byte *data;
	int width;
	int height;
	int channels;
} DgImageInfo;

DgImageInfo DgLoadImage(char* path);
