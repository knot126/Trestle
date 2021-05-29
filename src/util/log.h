/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Logging Facialites
 */ 

#pragma once

#include <stdio.h>
#include <stdarg.h>

#include "fs.h"

typedef enum DgLogLevel {
	DG_LOG_INFO = 0,
	DG_LOG_WARNING = 1,
	DG_LOG_ERROR = 2,
	DG_LOG_FATAL = 3,
} DgLogLevel;

typedef struct DgLogger {
	DgFile *log_file;
} DgLogger;

void DgLog(const DgLogLevel level, const char * const format, ...);
