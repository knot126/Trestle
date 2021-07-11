/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Physics Core
 */ 

#pragma once

typedef struct PhysicsSystem {
	
} PhysicsSystem;

void physics_new(PhysicsSystem *this);
void physics_update(PhysicsSystem *this);
void physics_free(PhysicsSystem *this);
