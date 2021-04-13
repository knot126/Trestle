/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Math Utilites: Vectors and Matricies
 */

#include <math.h>
#include <stdio.h>

// For C implementations that do not specify PI
#if !defined(M_PI)
	#define M_PI 3.14159265358979323846
#endif

#include "maths.h"

/*
 * Various angle operations
 */

float DgCos(float angle) {
	return (float) cos(angle * M_PI * 2.0f);
}

float DgSin(float angle) {
	return (float) sin(angle * M_PI * 2.0f);
}

float DgTan(float angle) {
	return (float) tan(angle * M_PI * 2.0f);
}

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

inline DgVec2 DgVec2New(float x, float y) {
	DgVec2 c;
	
	c.x = x;
	c.y = y;
	
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

inline DgVec3 DgVec3Scale(float a, DgVec3 b) {
	DgVec3 c;
	
	c.x = b.x * a;
	c.y = b.y * a;
	c.z = b.z * a;
	
	return c;
}

inline float DgVec3Magnitude(DgVec3 a) {
	if (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f) {
		return 0.0f;
	}
	
	return (float) sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
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
	
	float m = DgVec3Magnitude(a);
	
	if (m != 0.0f) {
		m = 1.0f / m;
	}
	
	c.x = a.x * m;
	c.y = a.y * m;
	c.z = a.z * m;
	
	return c;
}

inline DgVec3 DgVec3New(float x, float y, float z) {
	DgVec3 c;
	
	c.x = x;
	c.y = y;
	c.z = z;
	
	return c;
}

inline DgVec3 DgVec3Negate(DgVec3 a) {
	DgVec3 c;
	
	c.x = -a.x;
	c.y = -a.y;
	c.z = -a.z;
	
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

inline DgVec4 DgVec4New(float x, float y, float z, float w) {
	DgVec4 c;
	
	c.x = x;
	c.y = y;
	c.z = z;
	c.w = w;
	
	return c;
}

/* 
 * DgMat4
 */

inline DgVec4 DgMat4ByVec4Multiply(DgMat4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = (b.x * a.ax) + (b.x * a.ay) + (b.x * a.az) + (b.x * a.aw);
	c.y = (b.y * a.bx) + (b.y * a.by) + (b.y * a.bz) + (b.y * a.bw);
	c.z = (b.z * a.cx) + (b.z * a.cy) + (b.z * a.cz) + (b.z * a.cw);
	c.w = (b.y * a.dx) + (b.y * a.dy) + (b.y * a.dz) + (b.y * a.dw);
	
	return c;
}

inline DgMat4 DgMat4ByMat4Multiply(DgMat4 a, DgMat4 b) {
	DgMat4 c;
	
	c.ax = (a.ax * b.ax) + (a.ay * b.bx) + (a.az * b.cx) + (a.aw * b.dx);
	c.ay = (a.ax * b.ay) + (a.ay * b.by) + (a.az * b.cy) + (a.aw * b.dy);
	c.az = (a.ax * b.az) + (a.ay * b.bz) + (a.az * b.cz) + (a.aw * b.dz);
	c.aw = (a.ax * b.aw) + (a.ay * b.bw) + (a.az * b.cw) + (a.aw * b.dw);
	
	c.bx = (a.bx * b.ax) + (a.by * b.bx) + (a.bz * b.cx) + (a.bw * b.dx);
	c.by = (a.bx * b.ay) + (a.by * b.by) + (a.bz * b.cy) + (a.bw * b.dy);
	c.bz = (a.bx * b.az) + (a.by * b.bz) + (a.bz * b.cz) + (a.bw * b.dz);
	c.bw = (a.bx * b.aw) + (a.by * b.bw) + (a.bz * b.cw) + (a.bw * b.dw);
	
	c.cx = (a.cx * b.ax) + (a.cy * b.bx) + (a.cz * b.cx) + (a.cw * b.dx);
	c.cy = (a.cx * b.ay) + (a.cy * b.by) + (a.cz * b.cy) + (a.cw * b.dy);
	c.cz = (a.cx * b.az) + (a.cy * b.bz) + (a.cz * b.cz) + (a.cw * b.dz);
	c.cw = (a.cx * b.aw) + (a.cy * b.bw) + (a.cz * b.cw) + (a.cw * b.dw);
	
	c.dx = (a.dx * b.ax) + (a.dy * b.bx) + (a.dz * b.cx) + (a.dw * b.dx);
	c.dy = (a.dx * b.ay) + (a.dy * b.by) + (a.dz * b.cy) + (a.dw * b.dy);
	c.dz = (a.dx * b.az) + (a.dy * b.bz) + (a.dz * b.cz) + (a.dw * b.dz);
	c.dw = (a.dx * b.aw) + (a.dy * b.bw) + (a.dz * b.cw) + (a.dw * b.dw);
	
	return c;
}

inline DgMat4 DgMat4Inverse(DgMat4 a) {
	DgMat4 c;
	
	
	
	return c;
}

inline DgMat4 DgMat4Translate(DgMat4 a, DgVec3 b) {
	DgMat4 c = DgMat4New(1.0f);
	
	c.aw = b.x;
	c.bw = b.y;
	c.cw = b.z;
	
	a = DgMat4ByMat4Multiply(a, c);
	
	return a;
}

inline DgMat4 DgMat4Scale(DgMat4 a, DgVec3 b) {
	DgMat4 c = DgMat4New(1.0f);
	
	c.ax = b.x;
	c.by = b.y;
	c.cz = b.z;
	
	a = DgMat4ByMat4Multiply(a, c);
	
	return a;
}

inline DgMat4 DgMat4Rotate(DgMat4 a, DgVec3 b, float angle) {
	// TODO: Implement other rotations
	/* a = (the matrix that rotation will be applied to)
	 * b = (the axis that will be rotated upon)
	 */
	DgMat4 c = DgMat4New(1.0f);
	
	c.ax = DgCos(angle) + (b.x * b.x * (1 - DgCos(angle)));
	c.ay = (b.x * b.y * (1 - DgCos(angle))) - (b.z * DgSin(angle));
	c.az = (b.x * b.z * (1 - DgCos(angle))) + (b.y * DgSin(angle));
	
	c.bx = (b.y * b.x * (1 - DgCos(angle))) + (b.z * DgSin(angle));
	c.by = DgCos(angle) + (b.y * b.y * (1 - DgCos(angle)));
	c.bz = (b.y * b.z * (1 - DgCos(angle))) - (b.x * DgSin(angle));
	
	c.cx = (b.z * b.x * (1 - DgCos(angle))) - (b.y * DgSin(angle));
	c.cy = (b.z * b.y * (1 - DgCos(angle))) + (b.x * DgSin(angle));
	c.cz = DgCos(angle) + (b.z * b.z * (1 - DgCos(angle)));
	
	a = DgMat4ByMat4Multiply(a, c);
	
	return a;
}

inline DgMat4 DgMat4NewPerspective(float l, float r, float b, float t, float n, float f) {
	DgMat4 C = DgMat4New(1.0f);
	
	C.ax = (2 * n) / (r - l);
	C.az = (r + l) / (r - l);
	
	C.by = (2 * n) / (t - b);
	C.bz = (t + b) / (t - b);
	
	C.cz = -(f + n) / (f - n);
	C.cw = -(2 * f * n) / (f - n);
	
	C.dz = -1;
	
	return C;
}

inline DgMat4 DgMat4NewPerspective2(float fov, float rat, float f, float b) {
	/* Gives a perspective translation given FOV and Screen Ratio. */
	float tangent = DgTan(fov / 2);
	float h = f * tangent;
	float w = h * rat;
	
	return DgMat4NewPerspective(-w, w, -h, h, f, b);
}

inline DgMat4 DgMat4New(float a) {
	DgMat4 c;
	
	c.ax = a; c.ay = 0.0f; c.az = 0.0f; c.aw = 0.0f;
	c.bx = 0.0f; c.by = a; c.bz = 0.0f; c.bw = 0.0f;
	c.cx = 0.0f; c.cy = 0.0f; c.cz = a; c.cw = 0.0f;
	c.dx = 0.0f; c.dy = 0.0f; c.dz = 0.0f; c.dw = a;
	
	return c;
}

void DgMat4Print(DgMat4 a) {
	printf("⎡%f %f %f %f⎤\n", a.ax, a.ay, a.az, a.aw);
	printf("⎢%f %f %f %f⎥\n", a.bx, a.by, a.bz, a.bw);
	printf("⎢%f %f %f %f⎥\n", a.cx, a.cy, a.cz, a.cw);
	printf("⎣%f %f %f %f⎦\n", a.dx, a.dy, a.dz, a.dw);
}

/* 
 * Other misc. functions
 */

DgMat4 DgTransfromBruteCamera(DgVec3 trans, DgVec3 rot) {
	/**
	 * <summary>Brute-force camera transfrom in that </summary>
	 * <input type="DgVec3" name="trans">The translation that will be applied to the camera.</input>
	 * <input type="DgVec3" name="rot">The rotation that will be applied to the camera.</input>
	 */
	
	DgMat4 pos = DgMat4Translate(DgMat4New(1.0f), DgVec3New(-trans.x, -trans.y, -trans.z));
	
	DgMat4 rot_x = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(1.0f, 0.0f, 0.0f), rot.x);
	DgMat4 rot_y = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(0.0f, 1.0f, 0.0f), rot.y);
	DgMat4 rot_z = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(0.0f, 0.0f, 1.0f), rot.z);
	
	return DgMat4ByMat4Multiply(pos, DgMat4ByMat4Multiply(rot_x, DgMat4ByMat4Multiply(rot_y, rot_z)));
}

DgMat4 DgTransformLookAt(DgVec3 from, DgVec3 to, DgVec3 world_up) {
	/**
	 * <summary>Return a <type>DgMat4</type> that makes the camera look at <arg>to</arg>.</summary>
	 * <input type="DgVec3" name="from">Location of the camera.</input>
	 * <input type="DgVec3" name="to">The point where the camera faces.</input>
	 * <input type="DgVec3" name="world_up">The up-vector of the world.</input>
	 */
	DgMat4 view_matrix = DgMat4New(1.0f);
	DgMat4 rot_matrix = DgMat4New(1.0f);
	
	DgVec3 axis_n = DgVec3Normalise(DgVec3Subtract(to, from));
	DgVec3 axis_v = DgVec3Normalise(DgVec3Cross(axis_n, DgVec3Cross(world_up, axis_n)));
	DgVec3 axis_u = DgVec3Normalise(DgVec3Cross(axis_n, axis_v));
	
	// Rotate on the X-axis
	rot_matrix.ax = -axis_u.x;
	rot_matrix.ay = axis_u.y;
	rot_matrix.az = axis_u.z;
	
	// Rotate on the Y-axis
	rot_matrix.bx = axis_v.x;
	rot_matrix.by = -axis_v.y;
	rot_matrix.bz = axis_v.z;
	
	// Rotate on the Z-axis
	rot_matrix.cx = axis_n.x;
	rot_matrix.cy = -axis_n.y;
	rot_matrix.cz = axis_n.z;
	
	// Move the camera, but this does not angle it.
	view_matrix.aw = from.x;
	view_matrix.bw = -from.y;
	view_matrix.cw = from.z;
	
	view_matrix = DgMat4ByMat4Multiply(rot_matrix, view_matrix);
	
	return view_matrix;
}

DgMat4 DgTransformLookAt2(DgVec3 from, DgVec3 to, DgVec3 world_up) {
	/**
	 * <summary>Return a <type>DgMat4</type> that makes the camera look at <arg>to</arg>.</summary>
	 * <input type="DgVec3" name="from">Location of the camera.</input>
	 * <input type="DgVec3" name="to">The point where the camera faces.</input>
	 * <input type="DgVec3" name="world_up">The up-vector of the world.</input>
	 */
	DgMat4 view_matrix = DgMat4New(1.0f);
	DgMat4 rot_matrix = DgMat4New(1.0f);
	
	DgVec3 axis_n = DgVec3Normalise(DgVec3Subtract(to, from)); // forward
	DgVec3 axis_v = DgVec3Normalise(DgVec3Cross(world_up, axis_n)); // right
	DgVec3 axis_u = DgVec3Normalise(DgVec3Cross(axis_n, axis_v)); // up
	
	// Convert local X-axe to global X-axe
	rot_matrix.ax = -axis_v.x;
	rot_matrix.ay = axis_v.y;
	rot_matrix.az = axis_v.z;
	
	// Convert local Y-axe to global Y-axe
	rot_matrix.bx = axis_u.x;
	rot_matrix.by = -axis_u.y;
	rot_matrix.bz = axis_u.z;
	
	// Convert local Z-axe to global Z-axe
	rot_matrix.cx = -axis_n.x;
	rot_matrix.cy = axis_n.y;
	rot_matrix.cz = axis_n.z;
	
	// Move the camera, but this does not angle it.
	view_matrix.aw = from.x;
	view_matrix.bw = from.y;
	view_matrix.cw = from.z;
	
	view_matrix = DgMat4ByMat4Multiply(rot_matrix, view_matrix);
	
	return view_matrix;
}
