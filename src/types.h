/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Common types and global variables
 */

#pragma once

#include <inttypes.h>

/* 
 * DgCommonUnion: A union of all the common types.
 */

#ifdef DG_TRESTLE_USE_UINT8_AS_BYTE
	typedef uint8_t byte_t;
	typedef uint8_t byte;
#else
	typedef unsigned char byte_t;
	typedef unsigned char byte;
#endif

union DgCommonUnion {
	uint64_t asUInt64;
	double asDouble;
	uint32_t asUInt32;
	float asFloat;
	uint16_t asUInt16;
	uint8_t asUInt8;
	int64_t asInt64;
	int32_t asInt32;
	int16_t asInt16;
	int8_t asInt8;
	void *asVoids;
	char *asChars;
	byte *asBytes;
};

extern float g_physicsDelta;
extern float g_deltaTime;
