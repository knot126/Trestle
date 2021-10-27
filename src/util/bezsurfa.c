/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surface core implementation and anything needed for them not provided
 * by the maths library.
 * 
 * WARNING: The way that indices work in this library are a bit screwy since
 * they usually take (count - 1) instead of (count) as the length. This should
 * be reworked soon!
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
	
	float res = xfac(n) / (xfac(k) * xfac(n - k));
// 	DgLog(DG_LOG_VERBOSE, "(%f %f) = %f", n, k, res);
	return res;
}

float DgBersteinPolynomial(float n, float i, float t) {
	/**
	 * Calculate a berstien basis polynomial
	 * 
	 * @see https://mathworld.wolfram.com/BernsteinPolynomial.html
	 */
	
	float res = DgCombination(n, i) * xpow(1.0f - t, n - i) * xpow(t, i);
// 	DgLog(DG_LOG_VERBOSE, "(%f %f) * (1 - %f)^(%f - %f) * %f^%f = %f", n, i, t, n, i, t, i, res);
	return res;
}

DgVec3 DgBezCurveVec3(size_t n, DgVec3 *points, float u) {
	/**
	 * Calculate a bezier curve for a 3d object.
	 */
	
	DgVec3 s = (DgVec3) {0.0f, 0.0f, 0.0f};
	
	for (size_t i = 0; i <= n; i++) {
		s = DgVec3Add(s, DgVec3Scale(DgBersteinPolynomial(n, i, u), points[i]));
	}
	
	return s;
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
	
// 	for (size_t u = 0; u <= n; u++) {
// 		for (size_t v = 0; v <= m; v++) {
// 			printf("[(%d + 1) * %d + %d = %d](%f %f %f)  ", n, u, v, ((n + 1) * u) + v, points[(n * u) + v].x, points[(n * u) + v].y, points[(n * u) + v].z);
// 		}
// 		printf("\n");
// 	}
	
	DgVec3 mpoints[m + 1];
	
	for (size_t j = 0; j <= m; j++) {
		mpoints[j] = DgBezCurveVec3(n, &points[(n + 1) * j], u);
	}
	
	return DgBezCurveVec3(m, mpoints, v);
}
