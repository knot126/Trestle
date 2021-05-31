/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL Texture Management (mainly data structures)
 */

#pragma once

#include <inttypes.h>

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
OpenGLTextureManager gltexture_init(void);
void gltexture_free(OpenGLTextureManager *container);

bool gltexture_load_file(OpenGLTextureManager *container, const char * const name, char *path);
uint32_t gltexture_set_unit(OpenGLTextureManager *container, const char * const name, GLenum unit);
