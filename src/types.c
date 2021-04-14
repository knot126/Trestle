/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Common types and global variables
 */

#include "types.h"

const char* ASSETS_LOOK_PATH[4] = {
	"./assets",
	"../Engine/assets",
	"../Trestle/assets",
	"../assets",
};

const char* USER_LOOK_PATH[1] = {
	"$HOME",
};

float g_physicsDelta = 1.0f / 180.0f;
float g_deltaTime;
