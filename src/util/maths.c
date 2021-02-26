/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Math Utilites: Vectors and Matricies
 */

#include <math.h>

#include "maths.h"

/* 
 * DgVec2 
 */

inline DgVec2 DgVec2Add(DgVec2 a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	
	return c;
}

inline DgVec2 DgVec2Subtract(DgVec2 a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	
	return c;
}

inline DgVec2 DgVec2Multiply(DgVec2 a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	
	return c;
}

inline float DgVec2Magnitude(DgVec2 a) {
	return sqrt((a.x * a.x) + (a.y * a.y));
}

inline float DgVec2Dot(DgVec2 a, DgVec2 b) {
	return (a.x * b.x) + (a.y + b.y);
}

inline DgVec2 DgVec2Normalise(DgVec2 a) {
	DgVec2 c;
	
	float m = 1 / DgVec2Magnitude(a);
	
	c.x = a.x * m;
	c.y = a.y * m;
	
	return c;
}

/* 
 * DgVec3 
 */

inline DgVec3 DgVec3Add(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

inline DgVec3 DgVec3Subtract(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

inline DgVec3 DgVec3Multiply(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	
	return c;
}

inline float DgVec3Magnitude(DgVec3 a) {
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

inline float DgVec3Dot(DgVec3 a, DgVec3 b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline DgVec3 DgVec3Cross(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = (a.y * b.z) + (a.z * b.y);
	c.y = (a.z * b.x) + (a.x * b.z);
	c.z = (a.x * b.y) + (a.y * b.x);
	
	return c;
}

inline DgVec3 DgVec3Normalise(DgVec3 a) {
	DgVec3 c;
	
	float m = 1 / DgVec3Magnitude(a);
	
	c.x = a.x * m;
	c.y = a.y * m;
	c.z = a.z * m;
	
	return c;
}

/*
 * DgVec4
 */

inline DgVec4 DgVec4Add(DgVec4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
	
	return c;
}

inline DgVec4 DgVec4Subtract(DgVec4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
	
	return c;
}

inline DgVec4 DgVec4Multiply(DgVec4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
	
	return c;
}

inline float DgVec4Magnitude(DgVec4 a) {
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z) + (a.w * a.w));
}

inline float DgVec4Dot(DgVec4 a, DgVec4 b) {
	return (a.x * b.x) + (a.y + b.y) + (a.z * b.z) + (a.w * b.w);
}

inline DgVec4 DgVec4Normalise(DgVec4 a) {
	DgVec4 c;
	
	float m = 1 / DgVec4Magnitude(a);
	
	c.x = a.x / m;
	c.y = a.y / m;
	c.z = a.z / m;
	c.w = a.w / m;
	
	return c;
}
