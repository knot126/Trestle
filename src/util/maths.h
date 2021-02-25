/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Math Utilites: Vectors and Matricies
 */

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
	}
} DgVec4;

typedef DgVec4 DgColour;
