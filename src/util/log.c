/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Logging Facialites
 */

#include <stdio.h>
#include <stdarg.h>

#include "str.h"
#include "fs.h"

#include "log.h"

// enum { DG_LOG_BUFFER_SIZE = 256 };

// DgLogger *DG_MAIN_LOGGER__;

static void DgLogDumpBuffer(DgLogLevel level, const char * const buf) {
	
}

void DgLog(DgLogLevel level, const char * const format, ...) {
#if 0
	va_list args;
	int argc = 0;
	
	char buf[DG_LOG_BUFFER_SIZE] = { 0 };
	uint32_t buf_head = 0;
	
	// Find the count of params
	for (size_t i = 0; format[i] != '\0'; i++) {
		if (format[i] == '%') {
			if (format[i + 1] == '%') {
				i++; // skip that one too
				continue;
			}
			argc++;
		}
	}
	
	va_start(args, argc);
	
	for (size_t i = 0; format[i] != '\0'; i++) {
		if (format[i] == '%') {
			i++;
			
			if (format[i] == '%') {
				continue;
			}
			
			else if (format[i] == 'f') {
				
			}
		}
		
		else {
			
		}
	}
#endif
	
	puts(format);
	puts("\n");
}
