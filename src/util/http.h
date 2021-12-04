/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * HTTP Request and Response Composition Tools
 */ 

#pragma once

#include <inttypes.h>
#include <stdbool.h>

enum {
	DG_HTTP_INVALID = 0,
	DG_HTTP_GET,
	DG_HTTP_HEAD,
	DG_HTTP_POST,
	
	DG_HTTP_CONTINUE = 100,
	DG_HTTP_OK = 200,
	DG_HTTP_MOVED_PERMANENTLY = 301,
	DG_HTTP_TEMPORARY_REDIRECT = 307,
	DG_HTTP_NOT_IMPLEMENTED = 501,
	DG_HTTP_BAD_REQUEST = 400,
	DG_HTTP_UNAUTHORISED = 401,
	DG_HTTP_FORBIDDEN = 403,
	DG_HTTP_NOT_FOUND = 404,
	DG_HTTP_GONE = 410,
};

typedef struct DgHTTPRequest {
	uint32_t verb;
	const char *path;
} DgHTTPRequest;

typedef struct DgHTTPResponse {
	uint32_t null__;
} DgHTTPResponse;
