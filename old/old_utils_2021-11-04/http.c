/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * HTTP Request and Response Composition Tools
 */ 

#include "alloc.h"
#include "string.h"

#include "http.h"

uint32_t DgHTTPRequestNew(DgHTTPRequest *request, uint32_t verb, const char * const path) {
	/**
	 * Composes an HTTP request string. This must be freed using DgFree.
	 */
	
	memset(request, 0, sizeof *request);
	
	request->verb = verb;
	request->path = path;
	
	return 0;
}
