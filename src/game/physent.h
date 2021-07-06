/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics entity helper functions
 */

#pragma once

// Physics
bool entity_phys_set_flags(World * const restrict world, const uint32_t id, const int flags);
bool entity_phys_set_mass(World * const restrict world, const uint32_t id, const float mass);
bool entity_phys_add_force(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot);
