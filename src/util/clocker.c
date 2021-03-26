/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Time-based profiling
 */

#include <stdio.h>

#include "../util/time.h"

#include "clocker.h"

void DgClockerStart(float *c) {
	*c = DgTime();
}

void DgClockerMark(float *c, const char *name) {
	printf("%s took %fs.\n", name, DgTime() - *c);
}
