/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Error Handling
 */

#include <stdio.h>
#include <stdlib.h>

void DgFail(const char *msg, int err) {
	printf(msg);
	exit(err);
}
