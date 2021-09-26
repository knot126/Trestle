/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surface core implementation and anything needed for them not provided
 * by the maths library.
 */

#include "maths.h"

#include "bezsurfa.h"

float xbico(float n, float k) {
	/**
	 * Calculate a binomial coefficent
	 * 
	 * @see https://mathworld.wolfram.com/BinomialCoefficient.html
	 */
	
	return xfac(n) / (xfac(k) * xfac(n - k));
}

float xberpl(float n, float i, float t) {
	/**
	 * Calculate a berstien basis polynomial
	 * 
	 * @see https://mathworld.wolfram.com/BernsteinPolynomial.html
	 */
	
	return xbico(n, i) * xpow(1.0f - t, n - i) * xpow(t, i);
}

DgVec3 DgBezSurfVec3(uint64_t n, uint64_t m, DgVec3 *P, float u, float v) {
	/**
	 * Calculate the mapping from (u, v) to a point on a (n, m) order bezier
	 * surface.
	 * 
	 * NOTE: The array of points must be n + 1 by m + 1 otherwise there will be
	 * a segfault. If you store the order of a curve by its demensions, then
	 * please make sure to subtract one from each.
	 * 
	 * @see https://en.wikipedia.org/wiki/B%C3%A9zier_surface
	 */
	
	DgVec3 res = (DgVec3) {0.0f, 0.0f, 0.0f};
	
	for (uint64_t i = 1; i <= n; i++) {
		DgVec3 rowres = (DgVec3) {0.0f, 0.0f, 0.0f};
		
		for (uint64_t j = 1; j <= m; j++) {
			DgVec3 temp = DgVec3Scale(xberpl(n, i, u), DgVec3Scale(xberpl(m, j, v), P[(i * n) + j]));
			rowres = DgVec3Add(rowres, temp);
		}
		
		res = DgVec3Add(res, rowres);
	}
	
	return res;
}
