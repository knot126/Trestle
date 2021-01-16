/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the implementation of DgVec3.
 */

#include <math.h>

#include "vector3.h"

float DgVec3_mag(DgVec3 this) {
	/*
	 * Computes the magnitude of this vector and returns it.
	 */
	return sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
}

DgVec3 DgVec3_nor(DgVec3 this) {
	/*
	 * Returns the current vector as a normalised vector.
	 */
	DgVec3 v;
	
	float m = DgVec3_mag(this);
	
	if (this.x && this.y && this.z) {
		v.x = this.x / m;
		v.y = this.y / m;
		v.z = this.z / m;
	}
	
	return v;
}

DgVec3 DgVec3_scaled(DgVec3 this, float s) {
	/*
	 * Returns the current vector as a scaled vector.
	 */
	DgVec3 v;
	
	v.x = this.x * s;
	v.y = this.y * s;
	v.z = this.z * s;
	
	return v;
}

DgVec3 DgVec3_negated(DgVec3 this) {
	/*
	 * Negates the given Vector and returns the result.
	 */
	DgVec3 v;
	
	v.x = -this.x;
	v.y = -this.y;
	v.z = -this.z;
	
	return v;
}

float DgVec3_distance(DgVec3 this, DgVec3 a) {
	/*
	 * Computes the distance between two vectors (a and this) and returns it.
	 */
	DgVec3 b;
	
	b.x = a.x - this.x;
	b.y = a.y - this.y;
	b.z = a.z - this.z;
	
	float r = DgVec3_mag(b);
	
	return r;
}

float DgVec3_dot(DgVec3 this, DgVec3 a) {
	/*
	 * Computes the dot product of the current vector with the given vector.
	 */
	return (a.x * this.x) + (a.y * this.y) + (a.z * this.z);
}

DgVec3 DgVec3_cross(DgVec3 this, DgVec3 i) {
	/* 
	 * NOTE: not implemented, because I don't yet need it
	 * Computes the cross product of the current vector with the given vector.
	 */
	DgVec3 o;
	
	// ...
	
	return o;
}

float DgVec3_angle(DgVec3 this, DgVec3 a) {
	/*
	 * Computes the angle between two vectors.
	 */
	return acos(DgVec3_dot(this, a) / (DgVec3_mag(this) * DgVec3_mag(a)));
}

inline DgVec3 DgVec3_add(DgVec3 this, DgVec3 b) {
	/*
	 * Adds two 3D vectors and returns the result.
	 */
	DgVec3 c;
	
	c.x = this.x + b.x;
	c.y = this.y + b.y;
	c.z = this.z + b.z;
	
	return c;
}

inline DgVec3 DgVec3_sub(DgVec3 this, DgVec3 b) {
	/*
	 * Subtracts two 3D vectors and returns the result.
	 */
	DgVec3 c;
	
	c.x = this.x - b.x;
	c.y = this.y - b.y;
	c.z = this.z - b.z;
	
	return c;
}

inline DgVec3 DgVec3_mul(DgVec3 this, float a) {
	DgVec3 v;
	
	v.x = this.x * a;
	v.y = this.y * a;
	v.z = this.z * a;
	
	return v;
}

inline DgVec3 DgVec3_div(DgVec3 this, float a) {
	DgVec3 v;
	
	v.x = this.x / a;
	v.y = this.y / a;
	v.z = this.z / a;
	
	return v;
}
