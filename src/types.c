/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Common types and global variables
 */

#include "types.h"
#include "util/ini.h"

const char* USER_LOOK_PATH[1] = {
	"$HOME",
};

float g_physicsDelta = 1.0f / 180.0f;
float g_deltaTime;
const DgINIDocument * g_quickRunConfig;
