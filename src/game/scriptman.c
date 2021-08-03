/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game Script Manager
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

#include "scriptman.h"

void scriptman_new(ScriptManager *this) {
	/**
	 * Create a new game script object.
	 */
	
	memset(this, 0, sizeof *this);
}

void scriptman_free(ScriptManager *this) {
	/**
	 * Free a game script's allocated resources.
	 */
	
	
}

GameScriptHandle scriptman_create(ScriptManager *this) {
	/**
	 * Create a script object and initialise it.
	 */
	
	// Reallocate scripts if needed
	if (this->script_count >= this->script_alloc) {
		this->script_alloc = (this->script_alloc == 0) ? 8 : this->script_alloc * 2;
		this->script = DgRealloc(this->script, sizeof *this->script * this->script_alloc);
		
		if (!this->script) {
			return 0;
		}
	}
	
	this->script_count++;
	
	DgScript *s = &this->script[this->script_count - 1];
	
	// Initialise the script
	DgScriptInit(s);
	
	// Register the common functions
	DgRegisterRandFuncs(s);
	regiser_default_script_functions(s);
	
	DgScriptLoad(s, DgINIGet(g_quickRunConfig, "Main", "include_script_path", "assets://scripts/include.lua"));
	
	// Return handle (which is just `index + 1` for now)
	return this->script_count;
}

void scriptman_destroy(ScriptManager *this) {
	/**
	 * Destroy a script and call its free function.
	 */
	
	
}

void scriptman_update(ScriptManager *this, float delta) {
	/**
	 * Update all of the running scripts, passing the given delta time.
	 */
	
	
}
