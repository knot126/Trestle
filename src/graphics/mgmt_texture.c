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

#include "gl_incl.h"
#include "mgmt_texture.h"

OpenGLTextureManager gltexture_init(void) {
	return (OpenGLTextureManager) { .textures = 0, .count = 0 };
}

void gltexture_free(OpenGLTextureManager *container) {
	// Free textures on OpenGL's end
	for (uint32_t i = 0; i < container->count; i++) {
		glDeleteTexture(container->textures[i].value);
	}
	
	// Free keys that were dynamically allocated
	for (uint32_t i = 0; i < container->count; i++) {
		DgFree(container->textures[i].key);
	}
	
	DgFree(container->textures);
	
	container->textures = NULL;
	container->count = 0;
}

uint32_t gltexture_load_file(OpenGLTextureManager *container, const char * const name, char *path) {
	/**
	 * Load a texture from a file 
	 */
	
	uint32_t texture_id;
	
	glGenTextures(1, &texture_id);
	
	if (!texture_id) {
		return 0;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	DgImageInfo image = DgLoadImage(path);
	
	if (!image.data) {
		DgFail("Error: Failed to load texture.\n", 100);
	}
	
	GLenum channels;
	
	switch (image.channels) {
		case 1: channels = GL_RED; break;
		case 2: channels = GL_RG; break;
		case 3: channels = GL_RGB; break;
		case 4: channels = GL_RGBA; break;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, channels, image.width, image.height, 0, channels, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	DgFreeImage(&image);
	
	// store the texture ID
	container->count++;
	container->textures = (OpenGLTexturePair *) DgRealloc(container->textures, sizeof(OpenGLTexturePair) * container->count);
	
	if (!container->textures) {
		DgLog(DG_LOG_ERROR, "Could not (re)allocate texture list.");
		return 0;
	}
	
	container->textures[container->count - 1].key = DgStrdup(name);
	
	return texture_id;
}

void gltexture_set_unit(OpenGLTextureManager *container, const char * const name, GLenum unit) {
	/**
	 * Set a named texture as the active texutre
	 */
	
	glActiveTexture(unit);
	
	for (uint32_t i = 0; i < container->count; i++) {
		if (!strcmp(name, container->textures[i].key)) {
			glBindTexture(GL_TEXTURE_2D, container->textures[i].value);
			return;
		}
	}
	
	DgLog(DG_LOG_ERROR, "Could not find texture by name: '%s'.", name);
}
