/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Time Functions
 */

#include <time.h>
#include <inttypes.h>

#include "time.h"

struct timespec st;

void DgInitTime(void) {
	timespec_get(&st, TIME_UTC);
}

double DgTime(void) {
	/* 
	 * Returns the time in seconds since the program started.
	 * 
	 * TODO: Should t.tv_nsec be (t.tv_nsec - st.tv_nsec) instead? BUG?
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return (double) (t.tv_sec - st.tv_sec) + (t.tv_nsec / 1000000000.0);
}

double DgRealTime(void) {
	/*
	 * Returns the current real time in seconds
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return (double) (t.tv_sec) + (t.tv_nsec / 1000000000.0);
}

uint32_t DgNsecTime(void) {
	/*
	 * Get the current nanosecond part of the time.
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return t.tv_nsec;
}
