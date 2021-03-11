/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Config File Parser and Loader
 */

#pragma once

#include "../util/bag.h"

typedef enum DgINILoaderTokenType {
	DG_INI_NAME = 0,
	DG_INI_SECTION = 1,
	DG_INI_DELIMITER = 2,
	DG_INI_IGNORE = 3,
} DgINILoaderTokenType;

typedef struct DgINIToken {
	DgINILoaderTokenType type;
	char *data; // Note: There is not data for every token type
} DgINIToken;

typedef struct DgConfig {
	size_t size;
	char **sections;
	DgBag *configs;
} DgConfig;

DgConfig DgLoadConfigFile(char* path);
