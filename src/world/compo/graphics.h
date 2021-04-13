/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Graphics-system related functions
 */

#pragma once

bool entity_load_mesh(World *world, uint32_t id, char *path);
void world_set_camera(World * const world, const uint32_t id);
