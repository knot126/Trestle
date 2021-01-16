/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file contains the platform_main function for linux (and similar, 
 * unix-like platforms).
 */

#include <unistd.h>
#include <stdio.h>

int platform_main(int argc, char* argv[]) {
	printf("Using Linux platform_main\n");
	
	return 0;
}
