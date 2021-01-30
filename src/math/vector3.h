#pragma once

/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the implementation of vector3.
 */

typedef struct DgVec3 {
	/*
	 * This is a basic implementation of a 3D vector. This uses single-percision
	 * floating point numbers.
	 * 
	 * TODO: Other operations, and a version that uses doubles/int32/int64
	 */
	float x, y, z;
} DgVec3;
