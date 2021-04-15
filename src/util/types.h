/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Common types and global variables
 */

#pragma once

#ifdef DG_TRESTLE_USE_UINT8_AS_BYTE
	#include <inttypes.h>
#endif

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
