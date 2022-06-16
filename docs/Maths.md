# Maths

The Decent Games Maths Library is part of the Melon Framework and has a basic structure. It is written in pure C, so it does/cannot use things like operator overloading, function overloading and other things.

## Trigonometry

```c
float DgCos(float angle); // cos(θ)
float DgSin(float angle); // sin(θ)
float DgTan(float angle); // tan(θ)
```

Get the COSine, SINe or TANgent of angle θ.

## 2D and 3D Vectors

```c
typedef struct DgVec2 {
	float x, y;
} DgVec2;

typedef struct DgVec3 {
	float x, y, z;
} DgVec3;
```

Both the 2D and 3D vectors are pretty standard: access them using `.x`, `.y` and `.z` members.

### 2D Operations

```c
DgVec2 DgVec2Add(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Subtract(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Multiply(DgVec2 a, DgVec2 b);
float DgVec2Magnitude(DgVec2 a);
float DgVec2Dot(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Normalise(DgVec2 a);
DgVec2 DgVec2New(float x, float y);
```

 * `DgVec2New(float, float)`: Create a new, statically allocated vector.

### 3D Operations

```c
DgVec3 DgVec3Add(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Subtract(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Multiply(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Scale(float a, DgVec3 b);
float DgVec3Magnitude(DgVec3 a);
float DgVec3Dot(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Cross(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Normalise(DgVec3 a);
DgVec3 DgVec3New(float x, float y, float z);
DgVec3 DgVec3Negate(DgVec3 a);

DgVec3 DgVec3FromString(const char * const s);
```

 * `DgVec3Scale(float, DgVec3)`: Scale the vector and return the new vector. Same as the scalar multiplication of a vector.
 * `DgVec3Negate(DgVec3)`: Return the opposite vector.

## 4D Vectors

```c
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
```

The 4D vectors allow access as standard colour components as well as the normal x/y/z/w members.

### Operations

```c
DgVec4 DgVec4Add(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Subtract(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Multiply(DgVec4 a, DgVec4 b);
float DgVec4Magnitude(DgVec4 a);
float DgVec4Dot(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Normalise(DgVec4 a);
DgVec4 DgVec4New(float x, float y, float z, float w);
```
