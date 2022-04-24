/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game Script
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "util/alloc.h"
#include "util/script.h"
#include "util/str.h"
#include "util/rand.h"
#include "util/log.h"
#include "game/scripting.h"
#include "types.h"

#include "gamescript.h"

GameScript *game_script_active(GameScript *gs) {
	static GameScript *active;
	
	if (gs) {
		active = gs;
	}
	
	return active;
}

void game_script_new(GameScript *gs) {
	/**
	 * Create a new game script object.
	 */
	
	memset(gs, 0, sizeof *gs);
}

void game_script_free(GameScript *gs) {
	/**
	 * Free a game script's allocated resources.
	 */
	
	if (gs->active) {
		DgScriptCall(&gs->script, "free");
		DgScriptFree(&gs->script);
		gs->active = false;
	}
}

void game_script_yeild(GameScript * restrict gs, const char * restrict name) {
	/**
	 * Yeild to the script at the given path.
	 */
	
	gs->load_next = DgStrdup(name); // we might free the name string, so use a copy.
}

void game_script_update(GameScript *gs) {
	/**
	 * Update the game script as needed.
	 */
	
	if (gs->load_next) {
		char *next = gs->load_next;
		
		if (gs->active) {
			DgScriptCall(&gs->script, "free");
			DgScriptFree(&gs->script);
		}
		
		DgScriptInit(&gs->script);
		
		DgRegisterRandFuncs(&gs->script);
		regiser_default_script_functions(&gs->script);
		game_script_script_functions(&gs->script);
		
		DgScriptLoad(&gs->script, "assets://scripts/include.lua");
		DgScriptLoad(&gs->script, next);
		
		DgFree(next);
		gs->load_next = NULL;
		
		DgScriptCall(&gs->script, "init");
		
		gs->active = true;
	}
	
	if (gs->active) {
		int types[] = { DG_SCRIPT_NUMBER };
		DgScriptCallArgs(&gs->script, "tick", types, 1, g_deltaTime);
	}
}

static int scripted_YeildToScript(lua_State *script) {
	const char *name = lua_tostring(script, 1);
	
	game_script_yeild(game_script_active(NULL), name);
	
	return 0;
}

void game_script_script_functions(DgScript *script) {
	lua_register(script->state, "mgYeildToScript", &scripted_YeildToScript);
}
