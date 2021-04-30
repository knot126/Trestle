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
#include "graphics.h" // graphics utilities
#include "seg.h"

#include "scripting.h"

static int scripted_CreateEntity(lua_State *script) {
	uint32_t flags = lua_tointeger(script, 1);
	
	lua_pushinteger(script, world_create_entity(QuickRunActiveWorld, flags));
	
	return 1;
}

static int scripted_SetTransform(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	uint32_t stack = (uint32_t) lua_gettop(script);
	
	float px = (float) lua_tonumber(script, 2);
	float py = (float) lua_tonumber(script, 3);
	float pz = (float) lua_tonumber(script, 4);
	
	float rx, ry, rz, sx, sy, sz;
	
	if (stack > 4) {
		rx = (float) lua_tonumber(script, 5);
		ry = (float) lua_tonumber(script, 6);
		rz = (float) lua_tonumber(script, 7);
	}
	else {
		rx = 0.0f;
		ry = 0.0f;
		rz = 0.0f;
	}
	
	if (stack > 7) {
		sx = (float) lua_tonumber(script, 8);
		sy = (float) lua_tonumber(script, 9);
		sz = (float) lua_tonumber(script, 10);
	}
	else {
		sx = 1.0f;
		sy = 1.0f;
		sz = 1.0f;
	}
	
	entity_set_transform(QuickRunActiveWorld, id, DgVec3New(px, py, pz), DgVec3New(rx, ry, rz), DgVec3New(sx, sy, sz));
	
	lua_pushinteger(script, id);
	
	return 1;
}

static int scripted_AddForce(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	
	float px = (float) lua_tonumber(script, 2);
	float py = (float) lua_tonumber(script, 3);
	float pz = (float) lua_tonumber(script, 4);
	
	float rx = (float) lua_tonumber(script, 5);
	float ry = (float) lua_tonumber(script, 6);
	float rz = (float) lua_tonumber(script, 7);
	
	entity_phys_add_force(QuickRunActiveWorld, id, DgVec3New(px, py, pz), DgVec3New(rx, ry, rz));
	
	return 0;
}

static int scripted_SetMass(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	float mass = (float) lua_tonumber(script, 2);
	
	entity_phys_set_mass(QuickRunActiveWorld, id, mass);
	
	return 0;
}

static int scripted_LoadMesh(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	const char *path = lua_tostring(script, 2);
	
	entity_load_mesh(QuickRunActiveWorld, id, (char *) path);
	
	return 0;
}

static int scripted_LoadSegment(lua_State *script) {
	if (lua_gettop(script) != 1) {
		lua_pushnumber(script, 0.0);
		return 1;
	}
	
	const char *path = lua_tostring(script, 1);
	
	float length = segment_load(path);
	
	lua_pushinteger(script, length);
	
	return 1;
}

void registerWorldScriptFunctions(DgScript *script) {
	lua_register(script->state, "mgCreateEntity", &scripted_CreateEntity);
	lua_register(script->state, "mgTransform", &scripted_SetTransform);
	lua_register(script->state, "mgMesh", &scripted_LoadMesh);
	lua_register(script->state, "mgForce", &scripted_AddForce);
	lua_register(script->state, "mgMass", &scripted_SetMass);
	lua_register(script->state, "mgSegment", &scripted_LoadSegment);
}
