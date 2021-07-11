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
#include "graphics/mesh.h"
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

static int scripted_GetTransform(lua_State *script) {
	int top = lua_gettop(script);
	
	if (top != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_transform().");
		return 0;
	}
	
	Name name = lua_tointeger(script, 1);
	
	const Transform * const trans = graph_get(&(supervisor(NULL)->graph), name);
	
	if (!trans) {
		DgLog(DG_LOG_ERROR, "get_transform(%d): Failed to get transform.", name);
		return 0;
	}
	
	lua_pushnumber(script, trans->pos.x);
	lua_pushnumber(script, trans->pos.y);
	lua_pushnumber(script, trans->pos.z);
	lua_pushnumber(script, trans->rot.x);
	lua_pushnumber(script, trans->rot.y);
	lua_pushnumber(script, trans->rot.z);
	lua_pushnumber(script, trans->scale.x);
	lua_pushnumber(script, trans->scale.y);
	lua_pushnumber(script, trans->scale.z);
	
	return 9;
}

static int scripted_SetCamera(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of set_camera().");
		return 0;
	}
	
	graphics_set_camera(&(supervisor(NULL)->graphics), lua_tointeger(script, 1));
	
	return 0;
}

static int scripted_PushOBJMesh(lua_State *script) {
	if (lua_gettop(script) != 2) {
		DgLog(DG_LOG_ERROR, "Invalid usage of push_obj_mesh().");
		return 0;
	}
	
	graphics_load_obj_mesh(&(supervisor(NULL)->graphics), lua_tointeger(script, 1), lua_tostring(script, 2));
	
	return 0;
}

static int scripted_MakeBox(lua_State *script) {
	int top = lua_gettop(script);
	
	DgVec3 pos = {0.0f, 0.0f, 0.0f}, size = {1.0f, 1.0f, 1.0f}, col = {0.75f, 0.75f, 0.75f};
	const char * texture = NULL;
	
	if (top >= 3) {
		pos.x = lua_tonumber(script, 1);
		pos.y = lua_tonumber(script, 2);
		pos.z = lua_tonumber(script, 3);
	}
	
	if (top >= 6) {
		size.x = lua_tonumber(script, 4);
		size.y = lua_tonumber(script, 5);
		size.z = lua_tonumber(script, 6);
	}
	
	if (top >= 9) {
		col.x = lua_tonumber(script, 7);
		col.y = lua_tonumber(script, 8);
		col.z = lua_tonumber(script, 9);
	}
	
	if (top >= 10) {
		texture = lua_tostring(script, 11);
	}
	
	lua_pushinteger(script, make_box(supervisor(NULL), pos, size, col, texture));
	
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
	lua_register(script->state, "get_transform", &scripted_GetTransform);
	
	// Graphics
	lua_register(script->state, "set_camera", &scripted_SetCamera);
	lua_register(script->state, "push_obj_mesh", &scripted_PushOBJMesh);
	
	// Objects
	lua_register(script->state, "make_box", &scripted_MakeBox);
	
	// Input
	lua_register(script->state, "get_key", &scripted_GetKey);
}
