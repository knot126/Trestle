/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scripting Functions
 */

#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "util/script.h"
#include "input/input.h"
#include "game/box.h"

#include "scripting.h"

static int scripted_GetKey(lua_State *script) {
	if (lua_gettop(script) != 1) {
		return 0;
	}
	
	int code = lua_tointeger(script, 1);
	
	lua_pushboolean(script, getKeyPressed(code));
	
	return 1;
}

void registerWorldScriptFunctions(DgScript *script) {
	lua_register(script->state, "mgGetKey", &scripted_GetKey);
}
