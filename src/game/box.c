/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Box entity generation
 */

#include <string.h>

#include "global/supervisor.h"
#include "graphics/graphics.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/log.h"
#include "util/str.h"
#include "types.h"

#include "box.h"

Name make_box(Supervisor * const restrict sup, const DgVec3 pos, const DgVec3 size, const DgVec3 colour, const char * const texture) {
	Name name = sup_entity(sup, ENT_TRANSFORM | ENT_GRAPHICS_MESH | ENT_PHYSICS_OBJECT | ENT_PHYSICS_AABB);
	
	if (!name) {
		DgLog(DG_LOG_ERROR, "Failed to make new entity!");
		return 0;
	}
	
	Transform *trans = graph_get(&sup->graph, name);
	Mesh *mesh = graphics_get_mesh(&sup->graphics, name);
	
	// Push transform
	trans->pos = pos;
	trans->rot = (DgVec3) {0.0f, 0.0f, 0.0f};
	trans->scale = size;
	
	// Set AABB 
	physics_set_aabb(&sup->physics, name, size);
	
	// Generate box mesh
	const float s_BoxData[] = {
		// X      Y      Z     U     V     R     G     B
		// Front and Back
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		 
		-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		
		// Top and bottom
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		 
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		 
		// Left and right
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f,
	};
	
	const uint32_t s_IndexData[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};
	
	float *vertex = (float *) DgAlloc(sizeof(s_BoxData));
	
	if (!vertex) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", name);
		return 0;
	}
	
	memcpy(vertex, s_BoxData, sizeof(s_BoxData));
	
	uint32_t *index = (uint32_t *) DgAlloc(sizeof(s_IndexData));
	
	if (!index) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", name);
		DgFree(vertex);
		return 0;
	}
	
	memcpy(index, s_IndexData, sizeof(s_IndexData));
	
	// Set mesh data in mesh component
	mesh->vert = vertex;
	mesh->vert_count = 24;
	
	mesh->index = index;
	mesh->index_count = sizeof(s_IndexData) / sizeof(float);
	
	mesh->updated = true;
	
	// Update vertex data
	for (uint32_t i = 0; i < 24; i++) {
		vertex[(i * 8) + 5] = colour.x;
		vertex[(i * 8) + 6] = colour.y;
		vertex[(i * 8) + 7] = colour.z;
	}
	
	// Front and back
	vertex[(0 * 8) + 3] *= size.x;
	vertex[(0 * 8) + 4] *= size.y;
	vertex[(1 * 8) + 3] *= size.x;
	vertex[(1 * 8) + 4] *= size.y;
	vertex[(2 * 8) + 3] *= size.x;
	vertex[(2 * 8) + 4] *= size.y;
	vertex[(3 * 8) + 3] *= size.x;
	vertex[(3 * 8) + 4] *= size.y;
	
	vertex[(4 * 8) + 3] *= size.x;
	vertex[(4 * 8) + 4] *= size.y;
	vertex[(5 * 8) + 3] *= size.x;
	vertex[(5 * 8) + 4] *= size.y;
	vertex[(6 * 8) + 3] *= size.x;
	vertex[(6 * 8) + 4] *= size.y;
	vertex[(7 * 8) + 3] *= size.x;
	vertex[(7 * 8) + 4] *= size.y;
	
	// Top and bottom
	vertex[(8 * 8) + 3] *= size.x;
	vertex[(8 * 8) + 4] *= size.z;
	vertex[(9 * 8) + 3] *= size.x;
	vertex[(9 * 8) + 4] *= size.z;
	vertex[(10 * 8) + 3] *= size.x;
	vertex[(10 * 8) + 4] *= size.z;
	vertex[(11 * 8) + 3] *= size.x;
	vertex[(11 * 8) + 4] *= size.z;
	
	vertex[(12 * 8) + 3] *= size.x;
	vertex[(12 * 8) + 4] *= size.z;
	vertex[(13 * 8) + 3] *= size.x;
	vertex[(13 * 8) + 4] *= size.z;
	vertex[(14 * 8) + 3] *= size.x;
	vertex[(14 * 8) + 4] *= size.z;
	vertex[(15 * 8) + 3] *= size.x;
	vertex[(15 * 8) + 4] *= size.z;
	
	// Sides
	vertex[(16 * 8) + 3] *= size.y;
	vertex[(16 * 8) + 4] *= size.z;
	vertex[(17 * 8) + 3] *= size.y;
	vertex[(17 * 8) + 4] *= size.z;
	vertex[(18 * 8) + 3] *= size.y;
	vertex[(18 * 8) + 4] *= size.z;
	vertex[(19 * 8) + 3] *= size.y;
	vertex[(19 * 8) + 4] *= size.z;
	
	vertex[(20 * 8) + 3] *= size.y;
	vertex[(20 * 8) + 4] *= size.z;
	vertex[(21 * 8) + 3] *= size.y;
	vertex[(21 * 8) + 4] *= size.z;
	vertex[(22 * 8) + 3] *= size.y;
	vertex[(22 * 8) + 4] *= size.z;
	vertex[(23 * 8) + 3] *= size.y;
	vertex[(23 * 8) + 4] *= size.z;
	
	// Texture
	if (texture) {
		mesh->texture = DgStrdup(texture);
	}
	
	return name;
}
