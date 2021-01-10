/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the platform_main function for linux (and similar, 
 * unix-like platforms).
 */

#include <unistd.h>
#include <cstdio>

#include "plat/game.hpp"

int platform_main(int argc, char* argv[]) {
	printf("Using Linux platform_main\n");
	
	Game* game_instance = DgCreateGame();
	
	// ...
	
	if (!game_instance->init()) {
		return -1;
	}
	
	if (!game_instance->play()) {
		return 1;
	}
}
