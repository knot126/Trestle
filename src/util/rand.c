/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Random Numbers and Strings
 */

#include <math.h>
#include <inttypes.h>

#include "../util/time.h"

#include "rand.h"

uint32_t DgERandMidSqU32(void) {
	/* 
	 * Implementing middle squares as a test. DgE = do not use in production.
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

float DgERandMidSqF32(void) {
	/*
	 * Based on the random integer function, this will make a random floating
	 * point number.
	 */
	return (float) (DgERandMidSqU32() * (1.0f / 16777215.0f));
}
