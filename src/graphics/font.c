/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Stb_TrueType Wrapper
 */

#define STB_TRUETYPE_IMPLEMENTATION
#include "extern/stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

#include "util/fs.h"
#include "util/alloc.h"

#include "font.h"

void font_init(Font *this, const char * const path) {
	/**
	 * Load a TrueType font (.ttf) file.
	 */
	
}

void font_free(Font *this) {
	/**
	 * Free a font file.
	 */
	
}
