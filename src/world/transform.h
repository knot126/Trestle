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
bool entity_phys_set_flags(World *world, uint32_t id, int flags);
bool entity_phys_set_mass(World *world, uint32_t id, float mass);
bool entity_phys_add_force(World *world, uint32_t id, DgVec3 pos, DgVec3 rot);
