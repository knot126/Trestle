/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the platform_main function for linux (and similar, 
 * unix-like platforms).
 */

#include <unistd.h>
#include <stdio.h>

#include "../game.h"

int main(int argc, char* argv[]) {
	//printf("Using Linux platform_main\n");
	return game_main(argc, argv);
}
