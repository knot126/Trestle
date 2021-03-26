/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file is part of the Decent Games Engine. Please do not redistribute the
 * sources without prior permission from Decent Games and/or contributours.
 * 
 * Contributours:
 *  - cdde <cddepppp256@protonmail.com>
 * 
 * Main File
 * =========
 * 
 * This file holds the main function and therefore the call to platform_main.
 */

// Any "global" defines
#define GLEW_STATIC

// Platform_main will define the correct main for this platform
#ifdef __linux__
	#include "linux/main.h"
#endif

#ifdef __APPLE__
	#include "macos/main.h"
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
	#include "windows/main.h"
#endif
