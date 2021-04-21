/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scripting Support Utilities
 */

#include <stdbool.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "alloc.h"
#include "fs.h"

#include "script.h"

void DgScriptInit(DgScript *script) {
	/*
	 * Initialise the script
	 */
	script->state = luaL_newstate();
	
	luaL_openlibs(script->state);
}

bool DgScriptLoad(DgScript *script, char * const path) {
	/*
	 * Load a script file.
	 */
	char * real_path = DgEvalPath(path);
	
	if (!real_path) {
		return false;
	}
	
	luaL_dofile(script->state, real_path);
	
	DgFree(real_path);
	
	return true;
}

void DgScriptCall(DgScript *script, char *name) {
	/*
	 * Call a function in a script that has no arguments and no return values.
	 * Other functions should be called manually.
	 */
	lua_getglobal(script->state, name);
	
	lua_call(script->state, 0, 0);
	
	lua_pop(script->state, 0);
}

void DgScriptFree(DgScript *script) {
	/*
	 * Free script resources
	 */
	lua_close(script->state);
}
