/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * SimpleConfig File Lodader
 */

#pragma once

#include <stdbool.h>

#include "../util/bag.h"

typedef struct DgConfig {
	DgBag config;
} DgConfig;

DgConfig *DgConfigLoad(char *path, const bool enable_comments);
void DgConfigPrint(DgConfig *config);
void DgConfigFree(DgConfig *config);
