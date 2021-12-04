/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh Whisker - allows manitpulating meshes from Lua and other high-level
 * facilites
 */

#include <string.h>
#include <inttypes.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "graph/graph.h"
#include "global/supervisor.h"
#include "util/alloc.h"
#include "graphics/graphics.h"
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
	
	int top = lua_gettop(script);
	
	MeshWhisker *mw = lua_touserdata(script, 1);
	
	if (!mw) {
		QR_LUA_MW_ALLOC_ERROR();
	}
	
	float i = lua_tointeger(script, 2), j, k;
	
	if (top > 2) {
		j = lua_tointeger(script, 3);
		k = lua_tointeger(script, 4);
	}
	
	if ((mw->index_count + 2) >= mw->index_alloc) {
		mw->index_alloc = 4 + mw->index_alloc * 2;
		mw->index = DgRealloc(mw->index, mw->index_alloc);
		
		if (!mw->index) {
			QR_LUA_MW_ALLOC_ERROR();
		}
	}
	
	mw->index[mw->vertex_count++] = i;
	
	if (top > 2) {
		mw->index[mw->vertex_count++] = j;
		mw->index[mw->vertex_count++] = k;
	}
	
	return 0;
}

static int scripted_MeshWhiskerBind(lua_State *script) {
	/**
	 * Bind a mesh to an object.
	 */
	
	MeshWhisker *mw = lua_touserdata(script, 1);
	float id = lua_tointeger(script, 2);
	Mesh * const mesh = graphics_get_mesh(&supervisor(NULL)->graphics, id);
	
	// If there is an error, free and return nil
	if (!id || !mesh) {
		DgFree(mw->vertex);
		DgFree(mw->index);
		return 0;
	}
	
	// Otherwise, bind the mesh to the wisk.
	if (mesh->vert) {
		DgFree(mesh->vert);
	}
	
	if (mesh->index) {
		DgFree(mesh->index);
	}
	
	mesh->vert = (float *) mw->vertex;
	mesh->vert_count = mw->vertex_count;
	mesh->index = mw->index;
	mesh->index_count = mw->index_count;
	
	lua_pushinteger(script, id);
	
	return 1;
}

void register_meshwisk_functions(DgScript *script) {
	lua_register(script->state, "MeshWisk", &scripted_MeshWhiskerNew);
	lua_register(script->state, "mw_push_vertex", &scripted_MeshWhiskerPushVertex);
	lua_register(script->state, "mw_push_index", &scripted_MeshWhiskerPushIndex);
	lua_register(script->state, "mw_bind", &scripted_MeshWhiskerBind);
}

#undef QR_LUA_MW_ALLOC_ERROR
