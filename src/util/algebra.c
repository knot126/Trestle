/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Algebra related math functions
 */

#include <math.h>

float DgLinearInterpolate(float a, float b, float t) {
	/**
	 * Linear interpolation between two values.
	 */
	return ((1.0f - t) * a) + (t * b);
}
