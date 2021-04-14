/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Transfroms
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "../world/world.h"
#include "../util/maths.h"

bool entity_set_transform(World *world, uint32_t id, DgVec3 pos, DgVec3 rot, DgVec3 scale);
bool entity_set_physics(World *world, uint32_t id, DgVec3 pos, DgVec3 rot, DgVec3 scale);
