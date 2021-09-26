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

DgSurface3D *DgSurfaceInit(DgSurface3D *this, uint32_t rows, uint32_t cols);
void DgSurfaceFree(DgSurface3D *this);

DgVec3 DgSurface3DGetPoint(DgSurface3D *this, uint32_t x, uint32_t y);
void DgSurface3DSetPoint(DgSurface3D *this, uint32_t x, uint32_t y, DgVec3 *value);
DgVec3 DgSurface3DGetSample(DgSurface3D *this, uint32_t u, uint32_t v);
