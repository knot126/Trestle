/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh representation and tools
 */
 
#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "graphics/graphics.h"
#include "graphics/vertex1.h"
#include "types.h"

bool graphics_load_xml_mesh(GraphicsSystem * restrict this, Name id, const char * const restrict path);
bool graphics_load_obj_mesh(GraphicsSystem * restrict this, Name id, const char * const restrict path);
