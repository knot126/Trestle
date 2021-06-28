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

#include "util/script.h"
#include "util/log.h"
#include "compo.h"
#include "world.h"
#include "seg.h"

#include "scripting.h"

static int scripted_CreateEntity(lua_State *script) {
	uint32_t flags = lua_tointeger(script, 1);
	
	lua_pushinteger(script, world_create_entity(QuickRunActiveWorld, flags));
	
	return 1;
}

static int scripted_CreateUIElement(lua_State *script) {
	uint32_t flags = lua_tointeger(script, 1);
	
	lua_pushinteger(script, world_create_ui_element(QuickRunActiveWorld, flags));
	
	return 1;
}

static int scripted_DeleteOldEntity(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	
	lua_pushboolean(script, world_delete_older_than(world_active(NULL), id));
	
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

static int scripted_SetPhysicsFlags(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	uint32_t flags = lua_tointeger(script, 2);
	
	entity_phys_set_flags(QuickRunActiveWorld, id, flags);
	
	return 0;
}

static int scripted_LoadMesh(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	const char *path = lua_tostring(script, 2);
	
	entity_load_mesh(QuickRunActiveWorld, id, (char *) path);
	
	return 0;
}

static int scripted_LoadXMLMesh(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	const char *path = lua_tostring(script, 2);
	
	entity_load_xml_mesh(QuickRunActiveWorld, id, (char *) path);
	
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

static int scripted_CreateCamera(lua_State *script) {
	float px = 0.0f, py = 2.0f, pz = 3.0f;
	float rx = 0.1f, ry = 0.0f, rz = 0.0f;
	
	if (lua_gettop(script) >= 3) {
		px = (float) lua_tonumber(script, 1);
		py = (float) lua_tonumber(script, 2);
		pz = (float) lua_tonumber(script, 3);
	}
	
	if (lua_gettop(script) >= 6) {
		rx = (float) lua_tonumber(script, 4);
		ry = (float) lua_tonumber(script, 5);
		rz = (float) lua_tonumber(script, 6);
	}
	
	uint32_t ent = world_create_entity(QuickRunActiveWorld, QR_COMPONENT_TRANSFORM | QR_COMPONENT_CAMERA);
	entity_set_transform(QuickRunActiveWorld, ent, DgVec3New(px, py, pz), DgVec3New(rx, ry, rz), DgVec3New(1.0f, 1.0f, 1.0f));
	world_set_camera(QuickRunActiveWorld, ent);
	
	lua_pushinteger(script, ent);
	
	return 1;
}

static int scripted_AddBox(lua_State *script) {
	int top = lua_gettop(script);
	
	float 
		px = 0.0f, py = 0.0f, pz = 0.0f, 
		sx = 1.0f, sy = 1.0f, sz = 1.0f,
		cr = 1.0f, cg = 1.0f, cb = 1.0f;
	
	const char *texture = NULL;
	
	if (top >= 3) {
		px = (float) lua_tonumber(script, 1);
		py = (float) lua_tonumber(script, 2);
		pz = (float) lua_tonumber(script, 3);
	}
	
	if (top >= 6) {
		sx = (float) lua_tonumber(script, 4);
		sy = (float) lua_tonumber(script, 5);
		sz = (float) lua_tonumber(script, 6);
	}
	
	if (top >= 9) {
		cr = (float) lua_tonumber(script, 7);
		cg = (float) lua_tonumber(script, 8);
		cb = (float) lua_tonumber(script, 9);
	}
	
	if (top >= 10) {
		texture = lua_tostring(script, 10);
	}
	
	bool status = entity_generate_box(QuickRunActiveWorld, DgVec3New(px, py, pz), DgVec3New(sx, sy, sz), DgVec3New(cr, cg, cb), texture);
	
	lua_pushboolean(script, status);
	
	return 1;
}

static int scripted_AddBoxLevel(lua_State *script) {
	/**
	 * Adds a box to the level but makes sure that it is relitive to the start
	 * of the level.
	 */
	int top = lua_gettop(script);
	
	float 
		px = 0.0f, py = 0.0f, pz = 0.0f, 
		sx = 1.0f, sy = 1.0f, sz = 1.0f,
		cr = 1.0f, cg = 1.0f, cb = 1.0f;
	
	const char *texture = NULL;
	
	if (top >= 3) {
		px = (float) lua_tonumber(script, 1);
		py = (float) lua_tonumber(script, 2);
		pz = (float) lua_tonumber(script, 3);
	}
	
	if (top >= 6) {
		sx = (float) lua_tonumber(script, 4);
		sy = (float) lua_tonumber(script, 5);
		sz = (float) lua_tonumber(script, 6);
	}
	
	if (top >= 9) {
		cr = (float) lua_tonumber(script, 7);
		cg = (float) lua_tonumber(script, 8);
		cb = (float) lua_tonumber(script, 9);
	}
	
	if (top >= 10) {
		texture = lua_tostring(script, 10);
	}
	
	bool status = entity_generate_box(QuickRunActiveWorld, DgVec3New(px, py, pz - world_get_level_offset(world_active(NULL))), DgVec3New(sx, sy, sz), DgVec3New(cr, cg, cb), texture);
	
	lua_pushboolean(script, status);
	
	return 1;
}

static int scripted_SetPlayer(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	
	QuickRunActiveWorld->player_info.id = id;
	
	lua_pushinteger(script, id);
	
	return 1;
}

static int scripted_SetText(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	const char *string = lua_tostring(script, 2);
	
	bool success = ui_element_set_text(QuickRunActiveWorld, id, string);
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_SetTextPos(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	float x = (float) lua_tonumber(script, 2);
	float y = (float) lua_tonumber(script, 3);
	
	bool success = ui_element_set_text_pos(QuickRunActiveWorld, id, DgVec2New(x, y));
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_SetTextSize(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	float size = (float) lua_tonumber(script, 2);
	
	bool success = ui_element_set_text_size(QuickRunActiveWorld, id, size);
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_SetTextFont(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	const char *font = lua_tostring(script, 2);
	
	bool success = ui_element_set_text_font(QuickRunActiveWorld, id, font);
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_SetTextColour(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	float r = (float) lua_tonumber(script, 2);
	float g = (float) lua_tonumber(script, 3);
	float b = (float) lua_tonumber(script, 4);
	float a = (float) lua_tonumber(script, 5);
	
	bool success = ui_element_set_text_colour(QuickRunActiveWorld, id, DgVec4New(r, g, b, a));
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_SetBox(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	float px = (float) lua_tonumber(script, 2);
	float py = (float) lua_tonumber(script, 3);
	float sx = (float) lua_tonumber(script, 4);
	float sy = (float) lua_tonumber(script, 5);
	
	bool success = ui_element_set_box(QuickRunActiveWorld, id, DgVec2New(sx, sy), DgVec2New(px, py));
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_SetBoxColour(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	float r = (float) lua_tonumber(script, 2);
	float g = (float) lua_tonumber(script, 3);
	float b = (float) lua_tonumber(script, 4);
	float a = (float) lua_tonumber(script, 5);
	
	bool success = ui_element_set_box_colour(QuickRunActiveWorld, id, DgVec4New(r, g, b, a));
	
	lua_pushboolean(script, success);
	
	return 1;
}

static int scripted_GetEntCount(lua_State *script) {
	lua_pushinteger(script, QuickRunActiveWorld->mask_count);
	
	return 1;
}

static int scripted_GetPlayerPos(lua_State *script) {
	uint32_t id = lua_tointeger(script, 1);
	
	DgVec3 pos = world_get_player_position(QuickRunActiveWorld);
	
	lua_pushnumber(script, pos.x);
	lua_pushnumber(script, pos.y);
	lua_pushnumber(script, pos.z);
	
	return 3;
}

static int scripted_GetPlayerSpeed(lua_State *script) {
	lua_pushnumber(script, world_get_player_speed(QuickRunActiveWorld));
	
	return 1;
}

static int scripted_SetPlayerSpeed(lua_State *script) {
	float min = (float) lua_tonumber(script, 1);
	float max = (float) lua_tonumber(script, 2);
	
	world_set_speed(QuickRunActiveWorld, min, max);
	
	return 0;
}

static int scripted_ResetPlayer(lua_State *script) {
	lua_pushboolean(script, world_reset_player(QuickRunActiveWorld));
	
	return 1;
}

static int scripted_ClearWorld(lua_State *script) {
	world_destroy(QuickRunActiveWorld);
	
	return 0;
}

static int scripted_SetPaused(lua_State *script) {
	world_set_pause(QuickRunActiveWorld, lua_toboolean(script, 1));
	
	return 0;
}

static int scripted_PutLength(lua_State *script) {
	float number = (float) lua_tonumber(script, 1);
	
	world_put_length(QuickRunActiveWorld, number);
	
	return 0;
}

void registerWorldScriptFunctions(DgScript *script) {
	/*  Low-Level Entities  */
	lua_register(script->state, "mgEntity", &scripted_CreateEntity);
	lua_register(script->state, "mgDeleteOlder", &scripted_DeleteOldEntity);
	lua_register(script->state, "mgTransform", &scripted_SetTransform);
	lua_register(script->state, "mgMesh", &scripted_LoadMesh);
	lua_register(script->state, "mgMesh2", &scripted_LoadXMLMesh);
	lua_register(script->state, "mgForce", &scripted_AddForce);
	lua_register(script->state, "mgMass", &scripted_SetMass);
	lua_register(script->state, "mgPhysFlags", &scripted_SetPhysicsFlags);
	lua_register(script->state, "mgEntCount", &scripted_GetEntCount);
	lua_register(script->state, "mgClearWorld", &scripted_ClearWorld);
	lua_register(script->state, "mgPaused", &scripted_SetPaused);
	
	/* UI based entites */
	lua_register(script->state, "mgUIElement", &scripted_CreateUIElement);
	
	lua_register(script->state, "mgUIText", &scripted_SetText);
	lua_register(script->state, "mgUITextPos", &scripted_SetTextPos);
	lua_register(script->state, "mgUITextSize", &scripted_SetTextSize);
	lua_register(script->state, "mgUITextFont", &scripted_SetTextFont);
	lua_register(script->state, "mgUITextColour", &scripted_SetTextColour);
	
	lua_register(script->state, "mgUIBox", &scripted_SetBox);
	lua_register(script->state, "mgUIBoxColour", &scripted_SetBoxColour);
	
	/*  Segment and Level Management  */
	lua_register(script->state, "mgSegment", &scripted_LoadSegment);
	lua_register(script->state, "mgBox", &scripted_AddBox);
	lua_register(script->state, "mgBoxLevel", &scripted_AddBoxLevel);
	lua_register(script->state, "mgLength", &scripted_PutLength);
	
	/*  Higher-Level Entities  */
	lua_register(script->state, "mgCamera", &scripted_CreateCamera);
	lua_register(script->state, "mgActivePlayer", &scripted_SetPlayer);
	lua_register(script->state, "mgPlayerPos", &scripted_GetPlayerPos);
	lua_register(script->state, "mgResetPlayer", &scripted_ResetPlayer);
	
	lua_register(script->state, "mgGetSpeed", &scripted_GetPlayerSpeed);
	lua_register(script->state, "mgSetSpeed", &scripted_SetPlayerSpeed);
}
