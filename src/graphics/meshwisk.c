/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh Whisker - allows manitpulating meshes from Lua and other high-level
 * facilites
 */

#include <inttypes.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "util/alloc.h"
#include "graphics/mesh.h"

#include "meshwisk.h"

#define QR_LUA_MW_ALLOC_ERROR() lua_pushstring(script, "MeshWhisker: Allocation error!"); lua_error(script);

static int scripted_MeshWhiskerNew(lua_State *script) {
	/**
	 * Create a new MeshWhisker
	 */
	
	MeshWhisker* mw = (MeshWhisker *) lua_newuserdata(script, sizeof *mw);
	
	memset(mw, 0, sizeof *mw);
	
	return 1;
}

static int scripted_MeshWhiskerPushVertex(lua_State *script) {
	/**
	 * Push vertex data to a MeshWhisker
	 */
	
	MeshWhisker *mw = lua_touserdata(script, 1);
	
	if (!mw) {
		QR_LUA_MW_ALLOC_ERROR();
	}
	
	float x = lua_tonumber(script, 2);
	float y = lua_tonumber(script, 3);
	float z = lua_tonumber(script, 4);
	
	float u = lua_tonumber(script, 5);
	float v = lua_tonumber(script, 6);
	
	float r = lua_tonumber(script, 7);
	float g = lua_tonumber(script, 8);
	float b = lua_tonumber(script, 9);
	
	if (mw->vertex_count >= mw->vertex_alloc) {
		mw->vertex_alloc = 4 + mw->vertex_alloc * 2;
		mw->vertex = DgRealloc(mw->vertex, mw->vertex_alloc);
		
		if (!mw->vertex) {
			QR_LUA_MW_ALLOC_ERROR();
		}
	}
	
	mw->vertex[mw->vertex_count++] = (QRVertex1) {x,y,z,u,v,r,g,b};
	
	return 0;
}

static int scripted_MeshWhiskerPushIndex(lua_State *script) {
	/**
	 * Push index data to a MeshWhisker
	 */
	
	MeshWhisker *mw = lua_touserdata(script, 1);
	
	if (!mw) {
		QR_LUA_MW_ALLOC_ERROR();
	}
	
	float i = lua_tointeger(script, 2);
	
	if (mw->index_count >= mw->index_alloc) {
		mw->index_alloc = 4 + mw->index_alloc * 2;
		mw->index = DgRealloc(mw->index, mw->index_alloc);
		
		if (!mw->index) {
			QR_LUA_MW_ALLOC_ERROR();
		}
	}
	
	mw->index[mw->vertex_count++] = i;
	
	return 0;
}

static int scripted_MeshWhiskerBind(lua_State *script) {
	/**
	 * Bind a mesh to an object.
	 */
	
	MeshWhisker *mw = lua_touserdata(script, 1);
	float id = lua_tointeger(script, 2);
	
	// just free until new world system is ready
	DgFree(mw->vertex);
	DgFree(mw->index);
}

#undef QR_LUA_MW_ALLOC_ERROR
