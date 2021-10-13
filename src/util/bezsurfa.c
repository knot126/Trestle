/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surface core implementation and anything needed for them not provided
 * by the maths library.
 */

#include "maths.h"
#include "log.h"

#include "bezsurfa.h"

float DgCombination(float n, float k) {
	/**
	 * Calculate a binomial coefficent (combination)
	 * 
	 * @see https://mathworld.wolfram.com/BinomialCoefficient.html
	 * @see https://www.khanacademy.org/math/precalculus/x9e81a4f98389efdf:prob-comb/x9e81a4f98389efdf:combinations/v/introduction-to-combinations
	 */
	
	return xfac(n) / (xfac(k) * xfac(n - k));
}

float DgBersteinPolynomial(float n, float i, float t) {
	/**
	 * Calculate a berstien basis polynomial
	 * 
	 * @see https://mathworld.wolfram.com/BernsteinPolynomial.html
	 */
	
	return DgCombination(n, i) * xpow(1.0f - t, n - i) * xpow(t, i);
}

DgVec3 DgBezSurfVec3(size_t n, size_t m, DgVec3 *points, float u, float v) {
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
	
	DgVec3 s = (DgVec3) {0.0f, 0.0f, 0.0f};
	
	for (size_t i = 0; i <= n; i++) {
		DgVec3 r = (DgVec3) {0.0f, 0.0f, 0.0f};
		
		for (size_t j = 0; j <= m; j++) {
			r = DgVec3Add(r, DgVec3Scale(DgBersteinPolynomial(n, i, u) * DgBersteinPolynomial(m, j, v), points[(n * i) + j]));
		}
		
		s = DgVec3Add(s, r);
	}
	
	return s;
}
