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
#include "log.h"

#include "surface.h"

DgSurface3D *DgSurface3DInit(DgSurface3D * const restrict this, uint32_t rows, uint32_t cols) {
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

void DgSurface3DFree(DgSurface3D * restrict this) {
	/**
	 * Free resources assocaited with a surface.
	 */
	
	DgFree(this->points);
}

DgVec3 DgSurface3DGetPoint(const DgSurface3D * const restrict this, uint32_t x, uint32_t y) {
	/**
	 * Get a control point.
	 */
	
	return this->points[(y * (this->m + 1)) + x];
}

void DgSurface3DSetPoint(DgSurface3D * const restrict this, uint32_t x, uint32_t y, DgVec3 * restrict value) {
	/**
	 * Set a control point.
	 */
	
	this->points[(y * (this->m + 1)) + x] = *value;
}

DgVec3 DgSurface3DGetSample(const DgSurface3D * const restrict this, float u, float v) {
	/**
	 * Get a sample along the bezier surface.
	 */
	
	const DgVec3 p = DgBezSurfVec3(this->n, this->m, this->points, u, v);
	
// 	DgLog(DG_LOG_INFO, "Sample point = %f %f %f", p.x, p.y, p.z);
	
	return p;
}

void DgSurface3DGetBoundingBox(const DgSurface3D * const restrict this, DgVec3 * const restrict min, DgVec3 * const restrict max) {
	/**
	 * Get the bounding box of the surface.
	 */
	
	*min = (DgVec3) {100000000000000.0f, 100000000000000.0f, 100000000000000.0f};
	*max = (DgVec3) {-100000000000000.0f, -100000000000000.0f, -100000000000000.0f};
	
	for (size_t x = 0; x <= this->n; x++) {
		for (size_t y = 0; y <= this->m; y++) {
			DgVec3 cur = DgSurface3DGetPoint(this, x, y);
			
			if (cur.x >= max->x) {
				max->x = cur.x;
			}
			
			if (cur.y >= max->y) {
				max->y = cur.y;
			}
			
			if (cur.z >= max->z) {
				max->z = cur.z;
			}
			
			if (cur.x <= min->x) {
				min->x = cur.x;
			}
			
			if (cur.y <= min->y) {
				min->y = cur.y;
			}
			
			if (cur.z <= min->z) {
				min->z = cur.z;
			}
		}
	}
}

DgVec3 DgSurface3DGetBoundingBoxSize(const DgSurface3D * const restrict this) {
	/**
	 * Get the size of the bounding box containing control points.
	 */
	
	DgVec3 min, max;
	
	DgSurface3DGetBoundingBox(this, &min, &max);
	
	return DgVec3Subtract(max, min);
}
