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

// Unused constants
const char *strings[] = {
	" Quick Run and Trestle Engine Copyright (C) Decent Games, Knot Pecker 2021 ",
	" If you copy this game engine, please at least disclaim where you got it. ",
	" If you pirate this game, I will laugh at you for three seconds, and then not care. ",
	" #JusticeForTails "
};
