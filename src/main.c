/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file is part of the Decent Games Engine. Please do not redistribute the
 * sources without prior permission from Decent Games.
 * 
 * Contributours:
 *  - cdde <cddepppp256@protonmail.com>
 * 
 * Main File
 * =========
 * 
 * This file holds the main function and therefore the call to platform_main.
 */

// Platform_main will define the correct main for this platform
#ifdef __linux__
	#include "linux/main.h"
#endif

// Go to platform_main()
int main(int argc, char* argv[]) {
	return platform_main(argc, argv);
}
