/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Random Numbers and Strings
 */

#include <math.h>
#include <inttypes.h>

#include "time.h"

#include "rand.h"

static uint32_t DgERandMidSqU32(void) {
	/* 
	 * Implementing middle squares as a test. DgE = do not use in production.
	 * 
	 * DEPRECATED: Replaced with DgRandXORShiftSU32
	 */
	const uint32_t mask = 0b11111111111111111111111100000000;
	static uint32_t seed;
	
	if (seed == 0) {
		seed = ((uint32_t) (DgRealTime() * 1000.0f)) % 40000 + 1000;
	}
	
	seed = seed * seed;
	seed = (seed & mask) >> 8;
	
	return seed;
}

static float DgERandMidSqF32(void) {
	/*
	 * Based on the random integer function, this will make a random floating
	 * point number.
	 * 
	 * DEPRECATED: Replaced with DgRandXORShiftF32
	 */
	return (float) (DgERandMidSqU32() * (1.0f / 16777215.0f));
}

static uint32_t DgRandXORShiftU32(uint32_t n) {
	/*
	 * Generate a random number based on a given seed using the XOR-Shift 
	 * method of generating a random number.
	 */
	n ^= n << 13;
	n ^= n >> 15;
	n ^= n << 5;
	
	return n;
}

static uint32_t DgRandXORShiftSU32(void) {
	/*
	 * A pesudostateless version of the XOR-shift function. It should be 
	 * semi-threadsafe, since it will add a (most likely) unique value to make
	 * sure there are minimal conflicts.
	 */
	static uint32_t last;
	
	last = DgRandXORShiftU32(last + DgNsecTime());
	
	return last;
}

static float DgRandXORShiftF32(void) {
	return ((float) DgRandXORShiftSU32()) / 4294967295.0f;
}

uint32_t DgRandInt(void) {
	return DgRandXORShiftSU32();
}

float DgRandFloat(void) {
	return DgRandXORShiftF32();
}
