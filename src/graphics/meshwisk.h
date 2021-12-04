/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh Whisker - allows manitpulating meshes from Lua and other high-level
 * facilites
 */

#pragma once

#include <inttypes.h>

#include "graphics/mesh.h"

typedef struct {
	QRVertex1 *vertex;
	uint32_t vertex_count;
	uint32_t vertex_alloc;
	
	uint32_t *index;
	uint32_t  index_count;
	uint32_t  index_alloc;
	
	bool used;
} MeshWhisker;

void register_meshwisk_functions(DgScript *script);
