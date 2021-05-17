/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "../util/maths.h"

#include "compo.h"

typedef int32_t entity_t;
typedef entity_t Entity;

typedef uint32_t Mask;

typedef enum {
	QR_COMPONENT_TRANSFORM = (1 << 0),
	QR_COMPONENT_MESH = (1 << 1),
	QR_COMPONENT_CAMERA = (1 << 2),
	QR_COMPONENT_PHYSICS = (1 << 3),
	QR_COMPONENT_AABB = (1 << 4),
} ComponentMaskEnum;

typedef struct {
	Entity *trans;
	Entity *mesh;
	Entity *camera;
	Entity *phys;
} EntIndexes;

typedef struct {
	Entity id;
} PlayerWorld;

typedef struct {
	// Masks and entities
	EntIndexes ent;
	uint32_t   ent_count;
	
	// Transform Components
	C_Transform *trans;
	uint32_t     trans_count;
	
	// Mesh Components
	C_Mesh   *mesh;
	uint32_t  mesh_count;
	
	// Camera Components
	C_Camera *camera;
	uint32_t  camera_count;
	Entity camera_active;
	
	// Physics Components
	C_Physics *phys;
	uint32_t   phys_count;
	
	PlayerWorld player;
} World;

extern World *QuickRunActiveWorld;

// World
void world_init(World *world, size_t __UNUSED__);
void world_destroy(World *world);
Entity world_create_entity(World *world, uint32_t flags);
void SetActiveWorld(World *world);

// Transform
bool entity_set_transform(World * const restrict world, const Entity id, const DgVec3 pos, const DgVec3 rot, const DgVec3 scale);

// Mesh
bool entity_load_mesh(World * const restrict world, const Entity id, char * restrict path);
bool entity_generate_mesh_from_xml(World * const restrict world, const Entity id, const char * const restrict path);

// Camera
void world_set_camera(World * const restrict world, const Entity id);

// Physics
bool entity_phys_set_flags(World * const restrict world, const Entity id, const int flags);
bool entity_phys_set_mass(World * const restrict world, const Entity id, float mass);
bool entity_phys_add_force(World * const restrict world, const Entity id, const DgVec3 pos, const DgVec3 rot);
