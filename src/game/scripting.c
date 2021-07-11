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

#include "game/box.h"
#include "global/supervisor.h"
#include "util/log.h"
#include "util/script.h"
#include "input/input.h"
#include "game/box.h"
#include "types.h"

#include "scripting.h"

static int scripted_EntityName(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of next_name().");
		return 0;
	}
	
	lua_pushinteger(script, sup_next_name(supervisor(NULL)));
	
	return 1;
}

static int scripted_CreateEntity(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of create_entity().");
		return 0;
	}
	
	uint64_t systems = lua_tointeger(script, 1);
	
	lua_pushinteger(script, sup_entity(supervisor(NULL), systems));
	
	return 1;
}

static int scripted_PushTransform(lua_State *script) {
	int top = lua_gettop(script);
	
	if (top < 4) {
		DgLog(DG_LOG_ERROR, "Invalid usage of push_transform().");
		return 0;
	}
	
	DgVec3 pos = {0.0f, 0.0f, 0.0f}, rot = {0.0f, 0.0f, 0.0f}, scale = {1.0f, 1.0f, 1.0f};
	
	Name name = lua_tointeger(script, 1);
	
	pos.x = lua_tonumber(script, 2);
	pos.y = lua_tonumber(script, 3);
	pos.z = lua_tonumber(script, 4);
	
	if (top > 6) {
		rot.x = lua_tonumber(script, 5);
		rot.y = lua_tonumber(script, 6);
		rot.z = lua_tonumber(script, 7);
	}
	
	if (top > 9) {
		scale.x = lua_tonumber(script, 8);
		scale.y = lua_tonumber(script, 9);
		scale.z = lua_tonumber(script, 10);
	}
	
	graph_set(&(supervisor(NULL)->graph), name, (Transform) {pos, rot, scale});
	
	return 0;
}

static int scripted_SetCamera(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of set_camera().");
		return 0;
	}
	
	graphics_set_camera(&(supervisor(NULL)->graphics), lua_tointeger(script, 1));
	
	return 0;
}

static int scripted_MakeBox(lua_State *script) {
	int top = lua_gettop(script);
	
	DgVec3 pos = {0.0f, 0.0f, 0.0f}, size = {1.0f, 1.0f, 1.0f}, col = {0.75f, 0.75f, 0.75f};
	const char * texture = NULL;
	
	Name name = lua_tointeger(script, 1);
	
	if (top >= 3) {
		pos.x = lua_tonumber(script, 5);
		pos.y = lua_tonumber(script, 6);
		pos.z = lua_tonumber(script, 7);
	}
	
	if (top >= 6) {
		size.x = lua_tonumber(script, 5);
		size.y = lua_tonumber(script, 6);
		size.z = lua_tonumber(script, 7);
	}
	
	if (top >= 9) {
		col.x = lua_tonumber(script, 8);
		col.y = lua_tonumber(script, 9);
		col.z = lua_tonumber(script, 10);
	}
	
	if (top >= 10) {
		texture = lua_tostring(script, 11);
	}
	
	make_box(supervisor(NULL), pos, size, col, texture);
	
	return 0;
}

static int scripted_GetKey(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_key().");
		return 0;
	}
	
	int code = lua_tointeger(script, 1);
	
	lua_pushboolean(script, getKeyPressed(code));
	
	return 1;
}

void registerWorldScriptFunctions(DgScript *script) {
	// Supervisor and Entites
	lua_register(script->state, "next_name", &scripted_EntityName);
	lua_register(script->state, "create_entity", &scripted_CreateEntity);
	
	// Scene Graph
	lua_register(script->state, "push_transform", &scripted_PushTransform);
	
	// Graphics
	lua_register(script->state, "set_camera", &scripted_SetCamera);
	
	// Objects
	lua_register(script->state, "make_box", &scripted_MakeBox);
	
	// Input
	lua_register(script->state, "get_key", &scripted_GetKey);
}
