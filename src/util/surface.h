/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surface object
 */

#pragma once

#include "maths.h"

typedef struct DgSurface3D {
	DgVec3 *points;
	uint32_t n, m;
} DgSurface3D;

DgSurface *DgSurfaceInit(DgSurface *this, uint32_t rows, uint32_t cols);
void DgSurfaceFree(DgSurface *this);

