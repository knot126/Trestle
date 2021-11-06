/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL Texture Management (mainly data structures)
 */

#pragma once

#include <inttypes.h>

#include "util/bitmap.h"

// Main texture manager object
void gltexture_init(void *resman);
void gltexture_free(void *resman);

uint32_t gltexture_load_file(void *resman, const char * const name, char *path);
uint32_t gltexture_load_buffer(void *resman, const char * const name, DgBitmap *buffer);
void gltexture_set_unit(void *resman, const char * const name, unsigned unit);
uint32_t gltexture_get_name(void *resman, const char * const name);
void gltexture_load_list(void *resman, const char * const path);
