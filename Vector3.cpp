#include <cmath>
#include <string>

/** @class Vec3 */

class Vec3 {
public:
	/*
	 * This is a basic implementation of a 3D vector. This uses single-percision
	 * floating point numbers.
	 * 
	 * TODO: Other operations, and a version that uses doubles/int32/int64
	 */
	float x, y, z;
	
	float magnitude();
	Vec3 normalised();
	Vec3 scaled(float);
	Vec3 negated();
	float distance(Vec3);
	float dot(Vec3);
	Vec3 cross(Vec3);
	void set(float, float, float);
	Vec3 operator+(Vec3);
	Vec3 operator-(Vec3);
//	Vec3 operator*(Vec3);
	Vec3 operator*(float);
	Vec3 operator/(float a);
};

float Vec3::magnitude() {
	/*
	 * Computes the magnitude of this vector and returns it.
	 */
	return sqrt((x * x) + (y * y) + (z * z));
}

Vec3 Vec3::normalised() {
	/*
	 * Returns the current vector as a normalised vector.
	 */
	Vec3 v;
	
	float m = magnitude();
	
	if (!x || !y || !z) {
		v.x = x / m;
		v.y = y / m;
		v.z = z / m;
	}
	
	return v;
}

Vec3 Vec3::scaled(float s) {
	/*
	 * Returns the current vector as a scaled vector.
	 */
	Vec3 v;
	
	v.x = x * s;
	v.y = y * s;
	v.z = z * s;
	
	return v;
}

Vec3 Vec3::negated() {
	/*
	 * Negates the given Vector and returns the result.
	 */
	Vec3 v;
	
	v.x = -x;
	v.y = -y;
	v.z = -z;
	
	return v;
}

float Vec3::distance(Vec3 a) {
	/*
	 * Computes the distance between two vectors (a and this) and returns it.
	 */
	Vec3 b;
	
	b.x = a.x - x;
	b.y = a.y - y;
	b.z = a.z - z;
	
	float r = magnitude(b);
	
	return r;
}

float Vec3::dot(Vec3 a) {
	/*
	 * Computes the dot product of the current vector with the given vector.
	 */
	return a.x * x + b.y * y + b.z * z;
}

Vec3 Vec3::cross(Vec3 i) {
	/* 
	 * NOTE: not implemented
	 * Computes the cross product of the current vector with the given vector.
	 */
	Vec3 o;
	
	// ...
	
	return o;
}

float angle(Vec3 a) {
	/*
	 * Computes the angle between two vectors.
	 */
	return acos(dot(a) / (magnitude() * a.magnitude()));
}

void Vec3::set(float dx, float dy, float dz) {
	/*
	 * Sets this vector to a value.
	 */
	x = dx;
	y = dy;
	z = dz;
}

Vec3 Vec3::operator+(Vec3 b) {
	/*
	 * Adds two 3D vectors and returns the result.
	 */
	Vec3 c;
	
	c.x = x + b.x;
	c.y = y + b.y;
	c.z = z + b.z;
	
	return c;
}

Vec3 Vec3::operator-(Vec3 b) {
	/*
	 * Subtracts two 3D vectors and returns the result.
	 */
	Vec3 c;
	
	c.x = x - b.x;
	c.y = y - b.y;
	c.z = z - b.z;
	
	return c;
}

// Vec3 Vec3::operator*(Vec3 b) {
// 	/*
// 	 * Multiplies two 3D vectors and returns the result.
// 	 */
// 	Vec3 c;
// 	
// 	c.x = x * b.x;
// 	c.y = y * b.y;
// 	c.z = z * b.z;
// 	
// 	return c;
// }

Vec3 Vec3::operator*(float a) {
	Vec3 v;
	
	v.x = x * a;
	v.y = y * a;
	v.z = z * a;
	
	return v;
}

Vec3 Vec3::operator/(float a) {
	Vec3 v;
	
	v.x = x / a;
	v.y = y / a;
	v.z = z / a;
	
	return v;
}

/** @endclass Vec3 */

Vec3 getZeroVec3() {
	/* 
	 * Returns a zero vector
	 */
	Vec3 v;
	
	v.x = 0;
	v.y = 0;
	v.z = 0;
	
	return v;
}
