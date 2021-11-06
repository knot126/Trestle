/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL Texture Management (mainly data structures)
 */

#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "util/log.h"
#include "util/str.h"
#include "util/alloc.h"
#include "util/types.h"
#include "util/bitmap.h"

void gltexture_init(void *resman) {
	
}

void gltexture_free(void *resman) {
	/**
	 * Free the entire texture management structure.
	 */
	
}

uint32_t gltexture_load_file(void *resman, const char * const name, char *path) {
	/**
	 * Load a texture from a file 
	 */
	
	return 0;
}

uint32_t gltexture_load_buffer(void *resman, const char * const name, DgBitmap *buffer) {
	/**
	 * Load a texture from a DgBitmap buffer 
	 */
	
	return 0;
}

void gltexture_set_unit(void *resman, const char * const name, unsigned unit) {
	/**
	 * Set a named texture as the active texutre for the given texture unit
	 */
	
}

uint32_t gltexture_get_name(void *resman, const char * const name) {
	/**
	 * Get a texture's object handle ID from its name
	 */
	
	return 0;
}

void gltexture_load_list(void *resman, const char * const path) {
	/**
	 * Load a list of textures from an XML document containing a list of all the
	 * textures in the game that need to be loaded.
	 */
	
}
