/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Main functions
 */

#pragma once

#include "includes.h"

#include "scene/scene.h"
#include "graphics/graphics.h"

typedef struct {
	TrScene scene;
	
	TrGraphics graphics;
} TrEngine;
