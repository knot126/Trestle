/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "world/compo.h"
#include "world/gamestate.h"

typedef uint32_t mask_t;
typedef uint32_t ehand_t;

typedef enum {
	QR_COMPONENT_TRANSFORM = (1 << 0),
	QR_COMPONENT_MESH = (1 << 1),
	QR_COMPONENT_CAMERA = (1 << 2),
	QR_COMPONENT_PHYSICS = (1 << 3),
	QR_COMPONENT_AABB = (1 << 4),
} ComponentMaskEnum;

typedef struct {
	uint32_t id;
	float speed;
} PlayerWorld;

typedef struct {
	// Masks and entities
	
	// NOTE: As it turns out we don't need a mask list! We only need the current
	// entity count to get a unique ID! This makes deletion much easier!
	uint32_t mask_count;
	
	// Transform Components
	C_Transform *trans;
	uint32_t    trans_count;
	
	// Mesh Components
	C_Mesh   *mesh;
	uint32_t mesh_count;
	
	// Camera Components
	C_Camera *cam;
	uint32_t cam_count;
	uint32_t cam_active[3];
	
	// Physics Components
	C_Physics *phys;
	uint32_t  phys_count;
	
	// Other semiglobal state
	PlayerWorld player_info;
	GameState game;
	bool paused;
	
} World;

// Active world
extern World *QuickRunActiveWorld; // Accessing directly is deprecated
World *world_active(World * const restrict world);

// World create and destory
void world_init(World * const restrict world, size_t prealloc_count);
void world_destroy(World * const restrict world);
uint32_t world_create_entity(World * const restrict world, mask_t mask);
uint32_t world_create_ui_element(World * const restrict world, mask_t mask);
bool world_delete_older_than(World * const restrict world, uint32_t id);

// Helper functions
C_Transform *entity_find_trans(const World * const restrict world, const uint32_t id);
C_Mesh *entity_find_mesh(const World * const restrict world, const uint32_t id);
C_Camera *entity_find_cam(const World * const restrict world, const uint32_t id);
C_Physics *entity_find_phys(const World * const restrict world, const uint32_t id);

// Tranforms
bool entity_set_transform(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot, const DgVec3 scale);

// Cameras
void world_set_camera(World * const restrict world, const uint32_t id);

// Player position
DgVec3 world_get_player_position(const World * const restrict world);
float world_get_player_speed(const World * const restrict world);
bool world_reset_player(World * const restrict world);

// Pausing and game state
void world_set_pause(World * const restrict world, bool paused);
bool world_get_pause(World * const restrict world);
void world_get_speed(const World * const restrict world, float * restrict min, float * restrict max);
void world_set_speed(World * const restrict world, const float min, const float max);
void world_put_length(World * const restrict world, float number);
float world_get_level_offset(const World * const restrict world);
