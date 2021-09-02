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

Name make_rect(Supervisor * const restrict sup, const DgVec2 pos, const DgVec2 size, const DgVec4 colour) {
	Name name = sup_entity(sup, ENT_TRANSFORM | ENT_GRAPHICS_MESH2D);
	
	if (!name) {
		DgLog(DG_LOG_ERROR, "Failed to create entity in make_rect(sup = <0x%.16X>, ...)", sup);
		return 0;
	}
	
	// Push the transform
	Transform trans;
	
	trans.pos.x = pos.x;
	trans.pos.y = pos.y;
	
	graph_set(&sup->graph, name, trans);
	
	// Generate mesh and push
	QRVertex2D *vertex = DgAlloc(sizeof *vertex * 4);
	
	if (!vertex) {
		DgLog(DG_LOG_ERROR, "Failed to create entity in make_rect(sup = <0x%.16X>, ...)", sup);
		return 0;
	}
	
	uint32_t *index = DgAlloc(sizeof *index * 6);
	
	if (!index) {
		DgLog(DG_LOG_ERROR, "Failed to create entity in make_rect(sup = <0x%.16X>, ...)", sup);
		return 0;
	}
	
	graphics_set_mesh2d(&sup->graphics, name, 4, vertex, 6, index, NULL);
	
	return name;
}
