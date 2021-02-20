/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Image Tools
 */

#pragma once

typedef struct DgImageInfo {
	byte *data;
	uint16_t width;
	uint16_t height;
	uint16_t channels;
} DgImageInfo;

DgImageInfo DgLoadImage(const char* path, byte *datap);
