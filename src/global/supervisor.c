/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Supervisor for all the suptems
 */ 

#include <string.h>

#include "game/level.h"
#include "game/gamescript.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "util/log.h"
#include "util/ini.h"
#include "world/world.h"
#include "types.h"

#include "supervisor.h"

Supervisor *QR_MAIN_SUPERVISOR;

Supervisor *supervisor(Supervisor *S) {
	/**
	 * Get and set the main supervisor.
	 */
	
	if (S) {
		QR_MAIN_SUPERVISOR = S;
	}
	
	return QR_MAIN_SUPERVISOR;
}

void sup_init(Supervisor * restrict sup, const World * restrict world) {
	// Set to null
	memset(sup, 0, sizeof(Supervisor));
	
	// Graphics initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Graphics");
	graphics_init(&sup->graphics, world);
	graphics(&sup->graphics);
	
	// Input initialisation
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Input");
	input_init(&sup->input, &sup->graphics);
	
	// Run the main game script
	DgLog(DG_LOG_INFO, "Supervisor Initialise: GameScript");
	game_script_new(&sup->game_script);
	game_script_yeild(&sup->game_script, DgINIGet(g_quickRunConfig, "Main", "startup_script_path", "assets://scripts/startup.lua"));
	game_script_active(&sup->game_script);
	
	// Init the level manager
	DgLog(DG_LOG_INFO, "Supervisor Initialise: Levels");
	level_init(&sup->level_info, DgINIGet(g_quickRunConfig, "Resources", "level_index_path", "assets://game.xml"));
}

void sup_destroy(Supervisor * restrict sup) {
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Graphics");
	graphics_free(&sup->graphics);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Input");
	// Not currently required
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: GameScript");
	game_script_free(&sup->game_script);
	
	DgLog(DG_LOG_INFO, "Supervisor Destroy: Levels");
	level_free(&sup->level_info);
}
