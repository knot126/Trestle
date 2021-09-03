/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Rectangle entity generation
 */

#include <string.h>

#include "global/supervisor.h"
#include "graphics/graphics.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/log.h"
#include "util/str.h"

#include "rect.h"

Name make_rect(Supervisor * const restrict sup, const DgVec2 pos, const DgVec2 size, const DgVec4 colour, const char *texture) {
	Name name = sup_entity(sup, ENT_TRANSFORM | ENT_GRAPHICS_MESH2D);
	
	if (!name) {
		DgLog(DG_LOG_ERROR, "Failed to create entity in make_rect(sup = <0x%.16X>, ...)", sup);
		return 0;
	}
	
	// Push the transform
	Transform trans;
	
	trans.pos.x = pos.x;
	trans.pos.y = pos.y;
	trans.pos.z = 0.0f;
	
	trans.scale.x = size.x;
	trans.scale.y = size.y;
	trans.scale.z = 1.0f;
	
	graph_set(&sup->graph, name, trans);
	
	// Generate mesh and push
	QRVertex2D *vertex = DgAlloc(sizeof *vertex * 4);
	
	if (!vertex) {
		DgLog(DG_LOG_ERROR, "Failed to create entity in make_rect(sup = <0x%.16X>, ...)", sup);
		return 0;
	}
	
	vertex[0].x = -0.5f;
	vertex[0].y = 0.5f;
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].r = colour.r;
	vertex[0].g = colour.g;
	vertex[0].b = colour.b;
	vertex[0].a = colour.a;
	
	vertex[1].x = -0.5f;
	vertex[1].y = -0.5f;
	vertex[1].u = 0.0f;
	vertex[1].v = 0.0f;
	vertex[1].r = colour.r;
	vertex[1].g = colour.g;
	vertex[1].b = colour.b;
	vertex[1].a = colour.a;
	
	vertex[2].x = 0.5f;
	vertex[2].y = -0.5f;
	vertex[2].u = 0.0f;
	vertex[2].v = 0.0f;
	vertex[2].r = colour.r;
	vertex[2].g = colour.g;
	vertex[2].b = colour.b;
	vertex[2].a = colour.a;
	
	vertex[3].x = 0.5f;
	vertex[3].y = 0.5f;
	vertex[3].u = 0.0f;
	vertex[3].v = 0.0f;
	vertex[3].r = colour.r;
	vertex[3].g = colour.g;
	vertex[3].b = colour.b;
	vertex[3].a = colour.a;
	
	// Indexes
	uint32_t *index = DgAlloc(sizeof *index * 6);
	
	if (!index) {
		DgLog(DG_LOG_ERROR, "Failed to create entity in make_rect(sup = <0x%.16X>, ...)", sup);
		return 0;
	}
	
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	
	name = graphics_set_mesh2d(&sup->graphics, name, 4, vertex, 6, index, texture);
	
	return name;
}
