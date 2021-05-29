/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Error Handling
 */

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void DgFail(const char *msg, int err) {
	DgLog(DG_LOG_FATAL, msg);
	exit(err);
}
