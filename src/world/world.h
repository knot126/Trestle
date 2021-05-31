/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include "../world/compo.h"

typedef uint32_t mask_t;
typedef uint32_t ehand_t;

typedef enum {
	QR_COMPONENT_TRANSFORM = (1 << 0),
	QR_COMPONENT_MESH = (1 << 1),
	QR_COMPONENT_CAMERA = (1 << 2),
	QR_COMPONENT_PHYSICS = (1 << 3),
	QR_COMPONENT_AABB = (1 << 4),
} ComponentMaskEnum;

typedef enum {
	QR_ELEMUI_BOX = (1 << 0),
	QR_ELEMUI_TEXT = (1 << 1),
} UIElementMaskEnum;

typedef struct {
	uint32_t transform;
	uint32_t mesh;
	uint32_t camera;
	uint32_t physics;
} EntIndex;

typedef struct {
	uint32_t id;
} PlayerWorld;

typedef struct {
	/**
	 * The world for all UI elements.
	 */
	
	mask_t  *mask;
	uint32_t mask_count;
	
	C_UIBox *box;
	uint32_t box_count;
	
	C_UIText *text;
	uint32_t text_count;
} UIWorld;

typedef struct {
	// Masks and entities
	mask_t  *mask;
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
	
	PlayerWorld player_info;
	UIWorld *ui;
	
} World;

extern World *QuickRunActiveWorld;

void world_init(World *world, size_t prealloc_count);
void world_destroy(World *world);
uint32_t world_create_entity(World *world, mask_t mask);
uint32_t world_create_ui_element(World * const restrict world, mask_t mask);
void SetActiveWorld(World *world);

bool entity_load_mesh(World * const restrict world, uint32_t id, char * const restrict path);
void world_set_camera(World * const restrict world, const uint32_t id);

bool entity_set_transform(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot, const DgVec3 scale);
bool entity_phys_set_flags(World * const restrict world, const uint32_t id, const int flags);
bool entity_phys_set_mass(World * const restrict world, const uint32_t id, const float mass);
bool entity_phys_add_force(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot);

bool ui_element_set_text(World * const restrict world, const uint32_t id, const char * const restrict text);
bool ui_element_set_text_pos(World * const restrict world, const uint32_t id, const DgVec2 pos);
bool ui_element_set_text_size(World * const restrict world, const uint32_t id, const float size);

DgVec3 world_get_player_position(World * const restrict world);
