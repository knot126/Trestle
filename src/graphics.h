/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the graphics core.
 */

#include "math/vector2.h"

typedef struct {
	/* This class is responsible for holding the data for the window/surface. */
	DgVec2U size;
	char* title;
} DgSurfaceInfo;
