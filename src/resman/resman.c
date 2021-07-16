/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Resource Manager
 */

#include <string.h>
#include <inttypes.h>

#include "util/bitmap.h"
#include "util/alloc.h"
#include "util/hash.h"
#include "util/fs.h"

#include "resman.h"

/**
 * =============================================================================
 * Main Resource Manager: Init and Destory
 * =============================================================================
 */

void resman_init(ResMan *this) {
	memset(this, 0, sizeof *this);
}

#define QR_RESMAN_FREE_TYPE(NAME) \
	if (this->NAME ## _name) {\
		DgFree(this->NAME ## _name);\
	}\
	\
	if (this->NAME ## _hash) {\
		DgFree(this->NAME ## _hash);\
	}\
	\
	if (this->NAME) {\
		DgFree(this->NAME);\
	}

void resman_free(ResMan *this) {
	QR_RESMAN_FREE_TYPE(image);
}

/**
 * =============================================================================
 * Generic Function Implementations
 * =============================================================================
 */

#undef QR_RESMAN_FREE_TYPE

#define QR_RESMAN_TABLE_IMPLEMENTATION(NAME, TYPE)\
static uint8_t resman_list_ ## NAME ## _realloc(ResMan *this) {\
	if (this->NAME ## _count == this->NAME ## _alloc) {\
		this->NAME ## _alloc = 2 + this->NAME ## _alloc * 2;\
		\
		this->NAME = DgRealloc(this->NAME, sizeof *this->NAME * this->NAME ## _alloc);\
		\
		if (!this->NAME) {\
			return 1;\
		}\
		\
		this->NAME ## _hash = DgRealloc(this->NAME ## _hash, sizeof *this->NAME ## _hash * this->NAME ## _alloc);\
		\
		if (!this->NAME ## _hash) {\
			return 2;\
		}\
		\
		this->NAME ## _name = DgRealloc(this->NAME ## _name, sizeof *this->NAME ## _name * this->NAME ## _alloc);\
		\
		if (!this->NAME ## _name) {\
			return 3;\
		}\
	}\
	\
	return 0;\
}\
\
static size_t resman_list_ ## NAME ## _find(ResMan *this, const char *key) {\
	/**\
	 * Find the index of an image given the key.\
	 */\
	\
	uint32_t find = DgHashStringU32(key);\
	\
	for (size_t i = 0; i < this->NAME ## _count; i++) {\
		if (this->NAME ## _hash[i] == find) {\
			if (this->NAME ## _name[i] == key) {\
				return i;\
			}\
		}\
	}\
	\
	return -1;\
}\
\
static uint8_t resman_list_ ## NAME ## _add(ResMan *this, const char *key, TYPE value) {\
	/**\
	 * Add an image to the table, without checking if there is already one by\
	 * that name.\
	 */\
	\
	if (resman_list_ ## NAME ## _realloc(this)) {\
		return 1;\
	}\
	\
	++this->NAME ## _count;\
	\
	this->NAME ## _name[this->NAME ## _count - 1] = key;\
	this->NAME ## _hash[this->NAME ## _count - 1] = DgHashStringU32(key);\
	this->NAME[this->NAME ## _count - 1] = value;\
	\
	return 0;\
}

QR_RESMAN_TABLE_IMPLEMENTATION(image, DgBitmap);

#undef QR_RESMAN_TABLE_IMPLEMENTATION

/**
 * =============================================================================
 * Images and Textures
 * =============================================================================
 */

void resman_load_image(ResMan *this, const char * const path) {
	/**
	 * Load an image from the given path.
	 */
	
	
}
