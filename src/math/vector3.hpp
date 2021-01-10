/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the implementation of vector3.
 */

struct DgVec3 {
	/*
	 * This is a basic implementation of a 3D vector. This uses single-percision
	 * floating point numbers.
	 * 
	 * TODO: Other operations, and a version that uses doubles/int32/int64
	 */
	float x, y, z;
	
	float magnitude();
	DgVec3 normalised();
	DgVec3 scaled(float);
	DgVec3 negated();
	float distance(DgVec3);
	float dot(DgVec3);
	DgVec3 cross(DgVec3);
	void set(float, float, float);
	DgVec3 operator+(DgVec3);
	DgVec3 operator-(DgVec3);
	DgVec3 operator*(float);
	DgVec3 operator/(float a);
};

DgVec3 getZeroDgVec3();
