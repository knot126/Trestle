#include <cmath>

class Vector3 {
	/*
	 * This is a basic implementation of a 3D vector. This uses single-percision
	 * floating point numbers.
	 * 
	 * TODO: Other operations, and a version that uses doubles/int32/int64
	 */
	float x, y, z;
	
	float magnitude();
	Vector3 normalised();
	void normalise();
	Vector3 scaled(float s);
	void scale(float s);
};

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
	
	m = magnitude();
	v.x = x / m;
	v.y = y / m;
	v.z = z / m;
	
	return v;
}

void Vector3::normalise() {
	/*
	 * Normalises this vector.
	 */
	m = magnitude();
	x = x / m;
	y = y / m;
	z = z / m;
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

void Vector3::scale(float s) {
	/* 
	 * Scales the vector by scalar 's'
	 */
	x = x * s;
	y = y * s;
	z = z * s;
}

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
