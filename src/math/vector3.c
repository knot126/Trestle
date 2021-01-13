/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the implementation of DgVec3.
 */

#include <math.h>
#include "math/vector3.h"

/** @class DgVec3 */

float DgVec3_mag(DgVec3 this) {
	/*
	 * Computes the magnitude of this vector and returns it.
	 */
	return sqrt((x * x) + (y * y) + (z * z));
}

DgVec3 DgVec3_nor(DgVec3 this) {
	/*
	 * Returns the current vector as a normalised vector.
	 */
	DgVec3 v;
	
	float m = DgVec3_mag(this);
	
	if (x && y && z) {
		v.x = x / m;
		v.y = y / m;
		v.z = z / m;
	}
	
	return v;
}

DgVec3 DgVec3_scaled(DgVec3 this, float s) {
	/*
	 * Returns the current vector as a scaled vector.
	 */
	DgVec3 v;
	
	v.x = x * s;
	v.y = y * s;
	v.z = z * s;
	
	return v;
}

DgVec3 DgVec3_negated(DgVec3 this) {
	/*
	 * Negates the given Vector and returns the result.
	 */
	DgVec3 v;
	
	v.x = -x;
	v.y = -y;
	v.z = -z;
	
	return v;
}

float DgVec3_distance(DgVec3 this, DgVec3 a) {
	/*
	 * Computes the distance between two vectors (a and this) and returns it.
	 */
	DgVec3 b;
	
	b.x = a.x - x;
	b.y = a.y - y;
	b.z = a.z - z;
	
	float r = magnitude(b);
	
	return r;
}

float DgVec3_dot(DgVec3 this, DgVec3 a) {
	/*
	 * Computes the dot product of the current vector with the given vector.
	 */
	return a.x * x + b.y * y + b.z * z;
}

DgVec3 DgVec3_cross(DgVec3 this, DgVec3 i) {
	/* 
	 * NOTE: not implemented
	 * Computes the cross product of the current vector with the given vector.
	 */
	DgVec3 o;
	
	// ...
	
	return o;
}

float angle(DgVec3 this, DgVec3 a) {
	/*
	 * Computes the angle between two vectors.
	 */
	return acos(dot(a) / (magnitude() * a.magnitude()));
}

void DgVec3_set(DgVec3 this, float dx, float dy, float dz) {
	/*
	 * Sets this vector to a value.
	 */
	x = dx;
	y = dy;
	z = dz;
}

inline DgVec3 DgVec3_add(DgVec3 this, DgVec3 b) {
	/*
	 * Adds two 3D vectors and returns the result.
	 */
	DgVec3 c;
	
	c.x = x + b.x;
	c.y = y + b.y;
	c.z = z + b.z;
	
	return c;
}

inline DgVec3 DgVec3_sub(DgVec3 this, DgVec3 b) {
	/*
	 * Subtracts two 3D vectors and returns the result.
	 */
	DgVec3 c;
	
	c.x = x - b.x;
	c.y = y - b.y;
	c.z = z - b.z;
	
	return c;
}

inline DgVec3 DgVec3_mul(DgVec3 this, float a) {
	DgVec3 v;
	
	v.x = x * a;
	v.y = y * a;
	v.z = z * a;
	
	return v;
}

inline DgVec3 DgVec3_div(DgVec3 this, float a) {
	DgVec3 v;
	
	v.x = x / a;
	v.y = y / a;
	v.z = z / a;
	
	return v;
}
