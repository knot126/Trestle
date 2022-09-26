/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Common definitions for the graphics system
 */

#pragma once

#include "util/bitmap.h"
#include "util/window.h"

typedef struct {
	DgWindow window;
	DgBitmap bitmap;
} TrGraphics;

void TrGraphicsInit(TrGraphics *this, void *scene);
void TrGraphicsUpdate(TrGraphics *this, void *scene);
void TrGraphicsFree(TrGraphics *this, void *scene);
