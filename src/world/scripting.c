/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scripting Functions
 */

#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "../util/script.h"
#include "compo.h"
#include "world.h"
#include "transform.h"
#include "compo/graphics.h" // graphics utilities

#include "scripting.h"

static int scripted_CreateEntity(lua_State *script) {
	uint32_t flags = lua_tointeger(script, 1);
	printf("Create entity from lua using flags=%X.\n", flags);
	lua_pushinteger(script, world_create_entity(QuickRunActiveWorld, flags));
	return 1;
}

static int scripted_SetTransform(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	
	float px = (float) lua_tonumber(script, 2);
	float py = (float) lua_tonumber(script, 3);
	float pz = (float) lua_tonumber(script, 4);
	
	float rx = (float) lua_tonumber(script, 5);
	float ry = (float) lua_tonumber(script, 6);
	float rz = (float) lua_tonumber(script, 7);
	
	entity_set_transform(QuickRunActiveWorld, id, DgVec3New(px, py, pz), DgVec3New(rx, ry, rz), DgVec3New(1.0f, 1.0f, 1.0f));
	
	lua_pushinteger(script, id);
	
	return 1;
}

static int scripted_LoadMesh(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	const char *path = lua_tostring(script, 2);
	
	entity_load_mesh(QuickRunActiveWorld, id, (char *) path);
	
	return 0;
}

void registerWorldScriptFunctions(DgScript *script) {
	lua_register(script->state, "mgCreateEntity", &scripted_CreateEntity);
	lua_register(script->state, "mgSetTransform", &scripted_SetTransform);
	lua_register(script->state, "mgSetMesh", &scripted_LoadMesh);
}
