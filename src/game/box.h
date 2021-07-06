/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Box entity generation
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "util/maths.h"
#include "world/world.h"

bool entity_generate_box(World * const restrict world, const DgVec3 pos, const DgVec3 size, const DgVec3 colour, const char * const texture);
