/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Bèzier surfaces
 */

#pragma once

#include "maths.h"

float xbico(float n, float k);
float xberpl(float n, float i, float t);
DgVec3 DgBezSurfVec3(uint64_t n, uint64_t m, DgVec3 *P, float u, float v);
