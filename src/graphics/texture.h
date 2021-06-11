/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL Texture Management (mainly data structures)
 */

#pragma once

#include <inttypes.h>

#include "util/bitmap.h"
#include "gl_incl.h"

typedef struct OpenGLTexturePair {
	const char *key;
	uint32_t value;
} OpenGLTexturePair;

typedef struct OpenGLTextureManager {
	OpenGLTexturePair *textures;
	uint32_t count;
} OpenGLTextureManager;

// Main texture manager object
void gltexture_init(OpenGLTextureManager *resman);
void gltexture_free(OpenGLTextureManager *resman);

uint32_t gltexture_load_file(OpenGLTextureManager *resman, const char * const name, char *path);
uint32_t gltexture_load_buffer(OpenGLTextureManager *resman, const char * const name, DgBitmap *buffer);
void gltexture_set_unit(OpenGLTextureManager *resman, const char * const name, GLenum unit);
uint32_t gltexture_get_name(OpenGLTextureManager *resman, const char * const name);
