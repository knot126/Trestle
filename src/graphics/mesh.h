/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh representation and tools
 */
 
#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "world/world.h"

typedef struct QRVertex1 {
	float x, y, z;
	float u, v;
	float r, g, b;
} QRVertex1;

bool entity_load_mesh(World * const restrict world, uint32_t id, char * const restrict path);
bool entity_load_xml_mesh(World * const restrict world, uint32_t id, const char * const restrict path);
bool entity_load_obj_mesh(World * const restrict world, uint32_t id, const char * const restrict path);
