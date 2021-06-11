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

#include "gl_incl.h"
#include "image.h"

#include "texture.h"

void gltexture_init(OpenGLTextureManager *resman) {
	resman->textures = NULL;
	resman->count = 0;
}

void gltexture_free(OpenGLTextureManager *resman) {
	/**
	 * Free the entire texture management structure.
	 */
	
	// Free textures on OpenGL's end
	for (uint32_t i = 0; i < resman->count; i++) {
		glDeleteTextures(1, &resman->textures[i].value);
	}
	
	// Free keys that were dynamically allocated
	for (uint32_t i = 0; i < resman->count; i++) {
		DgFree((void *) resman->textures[i].key);
	}
	
	DgFree(resman->textures);
	
	resman->textures = NULL;
	resman->count = 0;
}

uint32_t gltexture_load_file(OpenGLTextureManager *resman, const char * const name, char *path) {
	/**
	 * Load a texture from a file 
	 */
	
	uint32_t texture_id;
	
	glGenTextures(1, &texture_id);
	
	if (!texture_id) {
		DgLog(DG_LOG_ERROR, "Failed to allocate OpenGL texture object whilst loading '%s'.\n", path);
		return 0;
	}
	
	// glBindTexture(GL_TEXTURE_2D, texture_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	DgImageInfo image = DgLoadImage(path);
	
	if (!image.data) {
		DgLog(DG_LOG_ERROR, "Failed to load texture at '%s'.\n", path);
		return 0;
	}
	
	GLenum channels;
	
	switch (image.channels) {
		case 1: channels = GL_RED; break;
		case 2: channels = GL_RG; break;
		case 3: channels = GL_RGB; break;
		case 4: channels = GL_RGBA; break;
		default: DgLog(DG_LOG_ERROR, "Could not determine feilds in bitmap!!"); break;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, channels, image.width, image.height, 0, channels, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	DgFreeImage(&image);
	
	// store the texture ID
	resman->count++;
	resman->textures = (OpenGLTexturePair *) DgRealloc(resman->textures, sizeof(OpenGLTexturePair) * resman->count);
	
	if (!resman->textures) {
		DgLog(DG_LOG_ERROR, "Could not (re)allocate texture list.");
		return 0;
	}
	
	resman->textures[resman->count - 1].key = DgStrdup(name);
	resman->textures[resman->count - 1].value = texture_id;
	
	return texture_id;
}

uint32_t gltexture_load_buffer(OpenGLTextureManager *resman, const char * const name, DgBitmap *buffer) {
	/**
	 * Load a texture from a DgBitmap buffer 
	 */
	
	uint32_t texture_id;
	
	glGenTextures(1, &texture_id);
	
	if (!texture_id) {
		DgLog(DG_LOG_ERROR, "Failed to allocate OpenGL texture object whilst loading from buffer.\n");
		return 0;
	}
	
	// glBindTexture(GL_TEXTURE_2D, texture_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLenum channels;
	
	switch (buffer->chan) {
		case 1: channels = GL_RED; break;
		case 2: channels = GL_RG; break;
		case 3: channels = GL_RGB; break;
		case 4: channels = GL_RGBA; break;
		default: DgLog(DG_LOG_ERROR, "Could not determine feilds in bitmap!!"); break;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, channels, buffer->width, buffer->height, 0, channels, GL_UNSIGNED_BYTE, buffer->src);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// store the texture ID
	resman->count++;
	resman->textures = (OpenGLTexturePair *) DgRealloc(resman->textures, sizeof(OpenGLTexturePair) * resman->count);
	
	if (!resman->textures) {
		DgLog(DG_LOG_ERROR, "Could not (re)allocate texture list.");
		return 0;
	}
	
	resman->textures[resman->count - 1].key = DgStrdup(name);
	resman->textures[resman->count - 1].value = texture_id;
	
	return texture_id;
}

void gltexture_set_unit(OpenGLTextureManager *resman, const char * const name, GLenum unit) {
	/**
	 * Set a named texture as the active texutre for the given texture unit
	 */
	
	glActiveTexture(unit);
	
	for (uint32_t i = 0; i < resman->count; i++) {
		if (!strcmp(name, resman->textures[i].key)) {
			glBindTexture(GL_TEXTURE_2D, resman->textures[i].value);
			DgLog(DG_LOG_VERBOSE, "Set %d to unit %d.", resman->textures[i].value, unit);
			return;
		}
	}
	
	DgLog(DG_LOG_ERROR, "Could not find texture by name: '%s'.", name);
}

uint32_t gltexture_get_name(OpenGLTextureManager *resman, const char * const name) {
	/**
	 * Get a texture's object handle ID from its name
	 */
	
	for (uint32_t i = 0; i < resman->count; i++) {
		if (!strcmp(name, resman->textures[i].key)) {
			return resman->textures[i].value - 1;
		}
	}
	
	DgLog(DG_LOG_ERROR, "Could not find texture by name: '%s'.", name);
	
	return 0;
}
