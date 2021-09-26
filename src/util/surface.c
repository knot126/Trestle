/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surface object
 */

#include <string.h>

#include "alloc.h"
#include "maths.h"
#include "bezsurfa.h"

#include "surface.h"

DgSurface3D *DgSurface3DInit(DgSurface3D *this, uint32_t rows, uint32_t cols) {
	/**
	 * Initialise a Bèzier surface of N by M degree or order. Returns pointer to
	 * the initialised surface on success or a zero on failure.
	 */
	
	memset(this, 0, sizeof *this);
	
	// Convert the number of rows and columns to the number of degrees in each
	// demension (subtract one to do this)
	this->n = rows - 1;
	this->m = cols - 1;
	
	// NOTE: Remember, order in n or m + 1 = number of rows or columns
	this->points = DgAlloc(rows * cols * sizeof *this->points);
	
	if (!this->points) {
		return NULL;
	}
	
	return this;
}

void DgSurface3DFree(DgSurface3D *this) {
	/**
	 * Free resources assocaited with a surface.
	 */
	
	DgFree(this->points);
}

DgVec3 DgSurface3DGetPoint(DgSurface3D *this, uint32_t x, uint32_t y) {
	/**
	 * Get a control point.
	 */
	
	return this->points[(y * (this->m + 1)) + x];
}

void DgSurface3DSetPoint(DgSurface3D *this, uint32_t x, uint32_t y, DgVec3 *value) {
	/**
	 * Set a control point.
	 */
	
	this->points[(y * (this->m + 1)) + x] = *value;
}

DgVec3 DgSurface3DGetSample(DgSurface3D *this, uint32_t u, uint32_t v) {
	/**
	 * Get a sample along the bezier surface.
	 */
	
	return DgBezSurfVec3(this->n, this->m, this->points, u, v);
}
