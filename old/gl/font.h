/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Stb_TrueType Wrapper
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "extern/stb_truetype.h"

typedef struct Font {
	stbtt_fontinfo font;
	unsigned char *data;
} Font;
