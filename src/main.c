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
 * This file holds the main function for each platform.
 */

// Any "global" defines
#define GLEW_STATIC

#include "game.h"

#ifdef __linux__
	int main(int argc, char* argv[]) {
		return game_main(argc, argv);
	}
#endif

#ifdef __APPLE__
	#error Building on macOS is not yet supported.
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define OS_WINDOWS
	#include "windows/main.h"
	
	int main(int argc, char *argv[]) {
		return game_main(argc, argv);
	}
#endif
