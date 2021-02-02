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
#include <stdlib.h>

#include "graphics/graphics.h"
#include "util/alloc.h"

void print_info() {
	printf("Engine compiled on %s at %s.\n", __DATE__, __TIME__);
}

int game_main() {
	/* The first real main game function, called from the main() function of the
	 * OS. */
	
	printf("Making initial memory pool (1 MiB)...\n");
	uint16_t mp = DgMakePool(1024 * 1024);
	
	printf("Init graphics subsystem...\n");
	DgVulkanInfo* vk = graphics_init();
	
	if (!vk) {
		printf("Error: Pointer to vulkan info is noll.\n");
		exit(1);
	}
	
	printf("Destroying graphics subsystem...");
	graphics_free(vk);
	printf("  Done\n");
	
	printf("Free memory pool...\n");
	DgFreePool(mp);
	
	return 0;
}
