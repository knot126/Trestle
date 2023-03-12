/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Common definitions for the graphics system
 */

#pragma once

#include "includes.h"

#include "util/bitmap.h"
#include "util/window.h"

#include "scene.h"

typedef struct {
	DgWindow window;
	DgBitmap bitmap;
} TrGraphics;

void TrGraphicsInit(TrGraphics *this, TrScene *scene);
void TrGraphicsUpdate(TrGraphics *this, TrScene *scene);
void TrGraphicsFree(TrGraphics *this, TrScene *scene);
