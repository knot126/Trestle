/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Math Utilites: Vectors and Matricies
 */

#pragma once

// Angle ops

float DgCos(float angle);
float DgSin(float angle);
float DgTan(float angle);

// DgVec**

typedef struct DgVec2 {
	float x, y;
} DgVec2;

typedef struct DgVec3 {
	float x, y, z;
} DgVec3;

typedef struct DgVec4 {
	union {
		float x, r;
	};
	union {
		float y, g;
	};
	union {
		float z, b;
	};
	union {
		float w, a;
	};
} DgVec4;

typedef DgVec4 DgColour;

// DgVec2

DgVec2 DgVec2Add(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Subtract(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Multiply(DgVec2 a, DgVec2 b);
float DgVec2Magnitude(DgVec2 a);
float DgVec2Dot(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Normalise(DgVec2 a);

// DgVec3

DgVec3 DgVec3Add(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Subtract(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Multiply(DgVec3 a, DgVec3 b);
float DgVec3Magnitude(DgVec3 a);
float DgVec3Dot(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Cross(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Normalise(DgVec3 a);

// DgVec4

DgVec4 DgVec4Add(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Subtract(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Multiply(DgVec4 a, DgVec4 b);
float DgVec4Magnitude(DgVec4 a);
float DgVec4Dot(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Normalise(DgVec4 a);

// DgMat**

typedef struct DgMat4x4 {
	float ax, ay, az, aw;
	float bx, by, bz, bw;
	float cx, cy, cz, cw;
	float dx, dy, dz, dw;
} DgMat4x4;

