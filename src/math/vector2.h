/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Vector2 implementation
 */

#pragma once

#include <inttypes.h>

typedef struct {
	float x, y;
} DgVec2;

typedef struct {
	double x, y;
} DgVec2D;

typedef struct {
	int32_t x, y;
} DgVec2I;

typedef struct {
	union {
		uint32_t x;
		uint32_t height;
	};
	
	union {
		uint32_t y;
		uint32_t width;
	};
} DgVec2U;

typedef struct {
	int64_t x, y;
} DgVec2I64;
