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

#include <stdio.h>

int print_info() {
	printf("Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

int game_main() {
	/* Fully abstracted main game function. */
}
