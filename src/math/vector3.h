/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vector3 implementation
 */

#pragma once

typedef struct DgVec3 {
	/*
	 * This is a basic implementation of a 3D vector. This uses single-percision
	 * floating point numbers.
	 * 
	 * TODO: Other operations, and a version that uses doubles/int32/int64
	 */
	float x, y, z;
} DgVec3;
