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
#include "game/rect.h"
#include "global/reg.h"
#include "global/supervisor.h"
#include "graphics/mesh.h"
#include "util/log.h"
#include "util/script.h"
#include "input/input.h"
#include "game/box.h"
#include "game/scriptman.h"
#include "types.h"

#include "scripting.h"

/**
 * =============================================================================
 * Supervisor and Entities
 * =============================================================================
 */

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

static int scripted_Quit(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of quit().");
		return 0;
	}
	
	sup_close(supervisor(NULL));
	
	return 0;
}

/**
 * =============================================================================
 * Scripts
 * =============================================================================
 */

static int scripted_ScriptCreate(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of script_create().");
		return 0;
	}
	
	lua_pushinteger(script, scriptman_create(&supervisor(NULL)->scriptman));
	
	return 1;
}

static int scripted_ScriptDestroy(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of script_destroy().");
		return 0;
	}
	
	scriptman_destroy(&supervisor(NULL)->scriptman, lua_tointeger(script, 1));
	
	return 0;
}

static int scripted_ScriptLoad(lua_State *script) {
	if (lua_gettop(script) != 2) {
		DgLog(DG_LOG_ERROR, "Invalid usage of script_load().");
		return 0;
	}
	
	lua_pushinteger(script, scriptman_load(&supervisor(NULL)->scriptman, lua_tointeger(script, 1), (char *) lua_tostring(script, 2)));
	
	return 1;
}

static int scripted_ScriptOpen(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of script_open().");
		return 0;
	}
	
	const char *path = lua_tostring(script, 1);
	
	lua_pushinteger(script, scriptman_open(&supervisor(NULL)->scriptman, (char *) path));
	
	return 1;
}

/**
 * =============================================================================
 * Scene Graph
 * =============================================================================
 */

static int scripted_CreateTransform(lua_State *script) {
	int top = lua_gettop(script);
	
	if (top != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of create_transform().");
		return 0;
	}
	
	lua_pushinteger(script, graph_create(&(supervisor(NULL)->graph), lua_tonumber(script, 1)));
	
	return 1;
}

static int scripted_SetTransform(lua_State *script) {
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

/**
 * =============================================================================
 * Graphics System
 * =============================================================================
 */

static int scripted_GraphicsUpdate(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of graphics_update().");
		return 0;
	}
	
	graphics_update(&(supervisor(NULL))->graphics, &(supervisor(NULL))->graph);
	
	lua_pushboolean(script, true);
}

static int scripted_GetCamera(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_camera().");
		return 0;
	}
	
	lua_pushinteger(script, graphics_get_camera(&(supervisor(NULL)->graphics)));
	
	return 1;
}

static int scripted_SetCamera(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of set_camera().");
		return 0;
	}
	
	graphics_set_camera(&(supervisor(NULL)->graphics), lua_tointeger(script, 1));
	
	return 0;
}

static int scripted_GetCameraForward(lua_State *script) {
	if (lua_gettop(script) != 3) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_camera_forward().");
		return 0;
	}
	
	DgVec3 result = graphics_get_camera_forward(&(supervisor(NULL)->graphics), &(supervisor(NULL)->graph), &(DgVec3) {lua_tonumber(script, 1), lua_tonumber(script, 2), lua_tonumber(script, 3)});
	
	lua_pushnumber(script, result.x);
	lua_pushnumber(script, result.y);
	lua_pushnumber(script, result.z);
	
	return 3;
}

static int scripted_SetBackground(lua_State *script) {
	if (lua_gettop(script) != 4) {
		DgLog(DG_LOG_ERROR, "Invalid usage of set_background().");
		return 0;
	}
	
	graphics_set_background(&(supervisor(NULL)->graphics), (DgVec4) {lua_tonumber(script, 1), lua_tonumber(script, 2), lua_tonumber(script, 3), lua_tonumber(script, 4)});
	
	return 0;
}

static int scripted_CreateMesh(lua_State *script) {
	int top = lua_gettop(script);
	
	if (top != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of create_mesh().");
		return 0;
	}
	
	lua_pushinteger(script, graphics_create_mesh(&(supervisor(NULL)->graphics), lua_tonumber(script, 1)));
	
	return 1;
}

static int scripted_GetShouldKeepOpen(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_should_keep_open().");
		return 0;
	}
	
	lua_pushboolean(script, get_should_keep_open(&(supervisor(NULL)->graphics)));
	
	return 1;
}

static int scripted_GetScreenSize(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_screen_size().");
		return 0;
	}
	
	DgVec2I size = graphics_get_screen_size(&(supervisor(NULL)->graphics));
	
	lua_pushinteger(script, size.x);
	lua_pushinteger(script, size.y);
	
	return 2;
}

static int scripted_SetMouseDisabled(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of set_mouse_disabled().");
		return 0;
	}
	
	graphics_set_mouse_disabled(&supervisor(NULL)->graphics, lua_toboolean(script, 1));
	
	return 0;
}

static int scripted_SetCameraFov(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of set_camera_fov().");
		return 0;
	}
	
	graphics_set_fov(&supervisor(NULL)->graphics, lua_tonumber(script, 1));
	
	return 0;
}

static int scripted_PushPatch(lua_State *script) {
	if (lua_gettop(script) <= 2) {
		DgLog(DG_LOG_ERROR, "Invalid usage of push_patch().");
		return 0;
	}
	
	Name name = lua_tointeger(script, 1);
	size_t x = lua_tointeger(script, 2), y = lua_tointeger(script, 3);
	
	DgVec3 points[x][y];
	
	size_t head = 4;
	
	for (size_t i = 0; i < x; i++) {
		for (size_t j = 0; j < y; j++) {
			points[i][j].x = lua_tonumber(script, head++);
			points[i][j].y = lua_tonumber(script, head++);
			points[i][j].z = lua_tonumber(script, head++);
		}
	}
	
	if (!graphics_create_patch_surface3d(&(supervisor(NULL)->graphics), name, x, y, (DgVec3 *) points)) {
		return 0;
	}
	
	lua_pushinteger(script, name);
	return 1;
}

/**
 * =============================================================================
 * Higher-level entites
 * =============================================================================
 */

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
		texture = lua_tostring(script, 10);
	}
	
	lua_pushinteger(script, make_box(supervisor(NULL), pos, size, col, texture));
	
	return 1;
}

static int scripted_MakeRect(lua_State *script) {
	int top = lua_gettop(script);
	
	DgVec2 pos = {0.0f, 0.0f}, size = {1.0f, 1.0f};
	DgVec4 col = {0.75f, 0.75f, 0.75f, 1.0f};
	const char * texture = NULL;
	
	if (top >= 2) {
		pos.x = lua_tonumber(script, 1);
		pos.y = lua_tonumber(script, 2);
	}
	
	if (top >= 4) {
		size.x = lua_tonumber(script, 3);
		size.y = lua_tonumber(script, 4);
	}
	
	if (top >= 8) {
		col.r = lua_tonumber(script, 5);
		col.g = lua_tonumber(script, 6);
		col.b = lua_tonumber(script, 7);
		col.a = lua_tonumber(script, 8);
	}
	
	if (top >= 9) {
		texture = lua_tostring(script, 9);
	}
	
	lua_pushinteger(script, make_rect(supervisor(NULL), pos, size, col, texture));
	
	return 1;
}

/**
 * =============================================================================
 * Registry
 * =============================================================================
 */

static int scripted_RegSet(lua_State *script) {
	if (lua_gettop(script) != 2) {
		DgLog(DG_LOG_ERROR, "Invalid usage of reg_set().");
		return 0;
	}
	
	registry_set(&(supervisor(NULL)->reg), lua_tostring(script, 1), (char *) lua_tostring(script, 2));
	
	return 0;
}

static int scripted_RegGet(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of reg_get().");
		return 0;
	}
	
	lua_pushstring(script, registry_get(&(supervisor(NULL)->reg), lua_tostring(script, 1)));
	
	return 1;
}

/**
 * =============================================================================
 * Input
 * =============================================================================
 */

static int scripted_GetKey(lua_State *script) {
	if (lua_gettop(script) != 1) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_key().");
		return 0;
	}
	
	int code = lua_tointeger(script, 1);
	
	lua_pushboolean(script, getKeyPressed(code));
	
	return 1;
}

static int scripted_GetMousePos(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_mouse_pos().");
		return 0;
	}
	
	DgVec2 pos = getMousePos();
	
	lua_pushnumber(script, pos.x);
	lua_pushnumber(script, pos.y);
	
	return 2;
}

static int scripted_GetMouseDelta(lua_State *script) {
	if (lua_gettop(script) != 0) {
		DgLog(DG_LOG_ERROR, "Invalid usage of get_mouse_delta().");
		return 0;
	}
	
	DgVec2 pos = getMouseDelta();
	
	lua_pushnumber(script, pos.x);
	lua_pushnumber(script, pos.y);
	
	return 2;
}

void regiser_default_script_functions(DgScript *script) {
	// Supervisor and Entites
	lua_register(script->state, "next_name", &scripted_EntityName);
	lua_register(script->state, "create_entity", &scripted_CreateEntity);
	lua_register(script->state, "quit", &scripted_Quit);
	
	// Script Management
	lua_register(script->state, "script_create", &scripted_ScriptCreate);
	lua_register(script->state, "script_destroy", &scripted_ScriptDestroy);
	lua_register(script->state, "script_load", &scripted_ScriptLoad);
	lua_register(script->state, "script_open", &scripted_ScriptOpen);
	
	// Scene Graph
	lua_register(script->state, "create_transform", &scripted_CreateTransform);
	lua_register(script->state, "set_transform", &scripted_SetTransform);
	lua_register(script->state, "get_transform", &scripted_GetTransform);
	
	// Graphics
	lua_register(script->state, "graphics_update", &scripted_GraphicsUpdate);
	
	lua_register(script->state, "set_camera", &scripted_SetCamera);
	lua_register(script->state, "get_camera", &scripted_GetCamera);
	lua_register(script->state, "get_camera_forward", &scripted_GetCameraForward);
	lua_register(script->state, "set_background", &scripted_SetBackground);
	lua_register(script->state, "get_should_keep_open", &scripted_GetShouldKeepOpen);
	lua_register(script->state, "get_screen_size", &scripted_GetScreenSize);
	lua_register(script->state, "set_mouse_disabled", &scripted_SetMouseDisabled);
	lua_register(script->state, "set_camera_fov", &scripted_SetCameraFov);
	
	lua_register(script->state, "create_mesh", &scripted_CreateMesh);
	lua_register(script->state, "push_patch", &scripted_PushPatch);
	
	// Input
	lua_register(script->state, "get_key", &scripted_GetKey);
	lua_register(script->state, "get_mouse_pos", &scripted_GetMousePos);
	lua_register(script->state, "get_mouse_delta", &scripted_GetMouseDelta);
	
	// Registry
	lua_register(script->state, "reg_get", &scripted_RegGet);
	lua_register(script->state, "reg_set", &scripted_RegSet);
	
	// Objects
	lua_register(script->state, "make_box", &scripted_MakeBox);
	lua_register(script->state, "make_rect", &scripted_MakeRect);
}
