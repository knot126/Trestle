/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surface core implementation and anything needed for them not provided
 * by the maths library.
 */

#pragma once

#include "maths.h"

float DgBersteinPolynomial(float n, float i, float t);
DgVec3 DgBezSurfVec3(size_t n, size_t m, DgVec3 *points, float u, float v);
