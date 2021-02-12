/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Time Functions
 */

#include <time.h>

#include "time.h"

struct timespec st;

void DgInitTime(void) {
	timespec_get(&st, TIME_UTC);
}

double DgTime(void) {
	/* 
	 * Returns the time in seconds since the program started.
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return (double) (t.tv_sec - st.tv_sec) + (t.tv_nsec / 1000000000.0);
}
