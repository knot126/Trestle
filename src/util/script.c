/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scripting Support Utilities
 */

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

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
	
	memset(script, 0, sizeof(DgScript));
	
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
	
	int stat = luaL_dofile(script->state, real_path);
	
	if (stat) {
		printf("\033[1;31mError:\033[0m Error whilst doing file %s, got errorcode %d.\n", path, stat);
	}
	
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

void DgScriptRegister(DgScript *script, const char * name, int (*function)(DgScript *)) {
	/**
	 * Register a C function of type (int (*)(DgScript *)) for calling.
	 */
	
	lua_register(script->state, name, (int (*)(lua_State *)) function);
}
