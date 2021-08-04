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

void scriptman_init(ScriptManager *this) {
	/**
	 * Create a new game script object.
	 */
	
	memset(this, 0, sizeof *this);
}

void scriptman_free(ScriptManager *this) {
	/**
	 * Free a game script's allocated resources.
	 */
	
	for (size_t i = 0; i < this->script_count; i++) {
		if (this->script_allocated[i] == true) {
			DgScriptFree(&this->script[i]);
		}
	}
	
	DgFree(this->script);
	DgFree(this->script_allocated);
}

static DgScript * const scriptman_get(ScriptManager *this, GameScriptHandle handle) {
	/**
	 * Get the reference to a script's state.
	 */
	
	return &this->script[handle - 1];
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
		
		this->script_allocated = DgRealloc(this->script_allocated, sizeof *this->script_allocated * this->script_alloc);
		
		if (!this->script) {
			return 0;
		}
	}
	
	this->script_count++;
	
	this->script_allocated[this->script_count - 1] = true;
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

void scriptman_destroy(ScriptManager *this, GameScriptHandle handle) {
	/**
	 * Destroy a script and call its free function.
	 */
	
	DgScript * const script = scriptman_get(this, handle);
	
	DgScriptCall(script, "free");
	
	DgScriptFree(script);
	this->script_allocated[handle - 1] = false;
}

uint32_t scriptman_load(ScriptManager * restrict this, GameScriptHandle handle, char * const restrict path) {
	/**
	 * Load a script file into a given script object.
	 */
	
	DgScript *script = scriptman_get(this, handle);
	
	if (!script) {
		return 1;
	}
	
	DgScriptLoad(script, path);
	
	return 0;
}

GameScriptHandle scriptman_open(ScriptManager * restrict this, char * const restrict path) {
	/**
	 * Create a new script state object and load a script file into it, also
	 * calling that script's init function if it exists.
	 */
	
	GameScriptHandle handle = scriptman_create(this);
	
	if (!handle) {
		return 0;
	}
	
	uint32_t failure = scriptman_load(this, handle, path);
	
	if (failure) {
		return 0;
	}
	
	DgScript *script = scriptman_get(this, handle);
	
	if (!script) {
		return 0;
	}
	
	DgScriptCall(script, "init");
	
	return handle;
}

void scriptman_update(ScriptManager *this, float delta) {
	/**
	 * Update all of the running scripts, passing the given delta time.
	 */
	
	int types[1] = { DG_SCRIPT_NUMBER };
	
	for (size_t i = 0; i < this->script_count; i++) {
		if (this->script_allocated[i] == true) {
			DgScriptCallArgs(&this->script[i], "tick", types, 1, delta);
		}
	}
}
