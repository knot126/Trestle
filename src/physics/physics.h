/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */ 

#pragma once

typedef struct PhysicsSystem {
	World *world;
} PhysicsSystem;

void physics_new(PhysicsSystem *this);
void physics_register_world(PhysicsSystem *this, World *world);
void physics_update(PhysicsSystem *this);
void physics_free(PhysicsSystem *this);
