/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the implementation of Vector3.
 */

#include <cmath>
#include <string>

/** @class Vector3 */

#include "math/vector3.hpp"

float Vector3::magnitude() {
	/*
	 * Computes the magnitude of this vector and returns it.
	 */
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::normalised() {
	/*
	 * Returns the current vector as a normalised vector.
	 */
	Vector3 v;
	
	float m = magnitude();
	
	if (x && y && z) {
		v.x = x / m;
		v.y = y / m;
		v.z = z / m;
	}
	
	return v;
}

Vector3 Vector3::scaled(float s) {
	/*
	 * Returns the current vector as a scaled vector.
	 */
	Vector3 v;
	
	v.x = x * s;
	v.y = y * s;
	v.z = z * s;
	
	return v;
}

Vector3 Vector3::negated() {
	/*
	 * Negates the given Vector and returns the result.
	 */
	Vector3 v;
	
	v.x = -x;
	v.y = -y;
	v.z = -z;
	
	return v;
}

float Vector3::distance(Vector3 a) {
	/*
	 * Computes the distance between two vectors (a and this) and returns it.
	 */
	Vector3 b;
	
	b.x = a.x - x;
	b.y = a.y - y;
	b.z = a.z - z;
	
	float r = magnitude(b);
	
	return r;
}

float Vector3::dot(Vector3 a) {
	/*
	 * Computes the dot product of the current vector with the given vector.
	 */
	return a.x * x + b.y * y + b.z * z;
}

Vector3 Vector3::cross(Vector3 i) {
	/* 
	 * NOTE: not implemented
	 * Computes the cross product of the current vector with the given vector.
	 */
	Vector3 o;
	
	// ...
	
	return o;
}

float angle(Vector3 a) {
	/*
	 * Computes the angle between two vectors.
	 */
	return acos(dot(a) / (magnitude() * a.magnitude()));
}

void Vector3::set(float dx, float dy, float dz) {
	/*
	 * Sets this vector to a value.
	 */
	x = dx;
	y = dy;
	z = dz;
}

Vector3 Vector3::operator+(Vector3 b) {
	/*
	 * Adds two 3D vectors and returns the result.
	 */
	Vector3 c;
	
	c.x = x + b.x;
	c.y = y + b.y;
	c.z = z + b.z;
	
	return c;
}

Vector3 Vector3::operator-(Vector3 b) {
	/*
	 * Subtracts two 3D vectors and returns the result.
	 */
	Vector3 c;
	
	c.x = x - b.x;
	c.y = y - b.y;
	c.z = z - b.z;
	
	return c;
}

Vector3 Vector3::operator*(float a) {
	Vector3 v;
	
	v.x = x * a;
	v.y = y * a;
	v.z = z * a;
	
	return v;
}

Vector3 Vector3::operator/(float a) {
	Vector3 v;
	
	v.x = x / a;
	v.y = y / a;
	v.z = z / a;
	
	return v;
}

/** @endclass Vector3 */

Vector3 getZeroVector3() {
	/* 
	 * Returns a zero vector
	 */
	Vector3 v;
	
	v.x = 0;
	v.y = 0;
	v.z = 0;
	
	return v;
}
