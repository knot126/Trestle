/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Resource Manager
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "util/bitmap.h"

#define QR_RESMAN_TABLE(NAME, TYPE) const char **NAME ## _name; \
	uint32_t   *NAME ## _hash; \
	TYPE       *NAME; \
	size_t      NAME ## _count; \
	size_t      NAME ## _alloc;

typedef struct ResMan {
	// Image
	QR_RESMAN_TABLE(image, DgBitmap);
} ResMan;

// Init and destroy
void resman_init(ResMan *this);
void resman_free(ResMan *this);

// Images
void resman_load_image(ResMan *this, const char * const path);
