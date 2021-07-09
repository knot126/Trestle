/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "compo.h"
#include "util/fs.h"
#include "util/alloc.h"
#include "util/fail.h"
#include "util/maths.h"
#include "util/log.h"
#include "util/str.h"
#include "util/obj.h"

#include "world.h"

World *QuickRunActiveWorld;

void world_init(World * const restrict world, size_t prealloc_count) {
	/*
	 * Creates a world full of entities
	 */
	
	DgLog(DG_LOG_DEPRECATION, "Traditional worlds have been depreacted, so try to referain from adding to the mess. See \"doc/World Struct Deprecation.md\" for more info.");
	
	memset(world, 0, sizeof(World));
}

void world_destroy(World * const restrict world) {
	/* 
	 * Frees the a world and its exsiting resources
	 */
	
	if (world->trans) {
		DgFree(world->trans);
	}
	
	if (world->mesh) {
		for (uint32_t i = 0; i < world->mesh_count; i++) {
			world_free_mesh_component(&world->mesh[i]);
		}
		
		DgFree(world->mesh);
	}
	
	if (world->cam) {
		DgFree(world->cam);
	}
	
	// Basically memset(world, 0, sizeof(World))
	uint8_t *b = (uint8_t *) world;
	for (size_t i = 0; i < sizeof(World); i++) {
		b[i] = 0;
	}
}

/**
 * -----------------------------------------------------------------------------
 * Entity creation and destruction
 * -----------------------------------------------------------------------------
 */

// TODO: Replace this with a code geneartion script at some point
#define QR_WORLD_ADDCOMPONENT(MASK, TYPE, LIST, LISTCOUNT) \
	if ((mask & MASK) == MASK) { \
		world->LISTCOUNT++; \
		world->LIST = DgRealloc(world->LIST, sizeof(TYPE) * world->LISTCOUNT); \
		 \
		if (!world->LIST) { \
			DgFail("Allocation error for world component list.\n", 2); \
		} \
		 \
		memset((world->LIST + (world->LISTCOUNT - 1)), 0, sizeof(TYPE)); \
		world->LIST[world->LISTCOUNT - 1].base.id = world->mask_count; \
	}

uint32_t world_create_entity(World * const restrict world, mask_t mask) {
	/*
	 * Create a new entity in a given world and return its ID
	 */
	world->mask_count++;
	
	// NOTE: nullptr passed to realloc works like alloc, so we do not need to
	// consider a case where the list has not been initialised.
	
	// Allocate the nessicary components
	
	QR_WORLD_ADDCOMPONENT(QR_COMPONENT_TRANSFORM, C_Transform, trans, trans_count);
	QR_WORLD_ADDCOMPONENT(QR_COMPONENT_MESH, C_Mesh, mesh, mesh_count);
	QR_WORLD_ADDCOMPONENT(QR_COMPONENT_CAMERA, C_Camera, cam, cam_count);
	QR_WORLD_ADDCOMPONENT(QR_COMPONENT_PHYSICS, C_Physics, phys, phys_count);
	
	return world->mask_count;
}

#undef QR_WORLD_ADDCOMPONENT

#define QR_FREE_OLD(TYPE, LIST, LISTCOUNT) for (size_t i = 1; i < world->LISTCOUNT; i++) {\
		if (world->LIST[i - 1].base.id > id) {\
			world->LISTCOUNT = i;\
			world->LIST = (TYPE *) DgRealloc(world->LIST, sizeof *world->LIST * i);\
			if (!world->LIST) {\
				DgLog(DG_LOG_ERROR, "Failed to free some older items in the world!!");\
				return false;\
			}\
			break;\
		}\
	}

bool world_delete_older_than(World * const restrict world, uint32_t id) {
	/**
	 * Frees all entities with an ID that is higher than the given id.
	 */
	
	QR_FREE_OLD(C_Transform, trans, trans_count);
	QR_FREE_OLD(C_Mesh, mesh, mesh_count);
	QR_FREE_OLD(C_Camera, cam, cam_count);
	QR_FREE_OLD(C_Physics, phys, phys_count);
	
	return true;
}

#undef QR_FREE_OLD

/**
 * -----------------------------------------------------------------------------
 * Helper functions
 * -----------------------------------------------------------------------------
 */

// NOTE: These should use a code geneator someday...

#define QR_DECL_FIND_ENT_COMPO(FUNCNAME, TYPE, NAME, COUNT) TYPE *FUNCNAME(const World * const restrict world, const uint32_t id) { \
	TYPE *compo = NULL; \
	for (int i = 0; i < world->COUNT; i++) { \
		if (world->NAME[i].base.id == id) {\
			compo = &world->NAME[i];\
			break;\
		}\
	}\
	return compo; \
} \

QR_DECL_FIND_ENT_COMPO(entity_find_trans, C_Transform, trans, trans_count);
QR_DECL_FIND_ENT_COMPO(entity_find_mesh, C_Mesh, mesh, mesh_count);
QR_DECL_FIND_ENT_COMPO(entity_find_cam, C_Camera, cam, cam_count);
QR_DECL_FIND_ENT_COMPO(entity_find_phys, C_Physics, phys, phys_count);

#undef QR_DECL_FIND_ENT_COMPO

/**
 * -----------------------------------------------------------------------------
 * Active world setters and getters
 * -----------------------------------------------------------------------------
 */

World *world_active(World * const restrict world) {
	/**
	 * If world == NULL, then get active world.
	 * If world != NULL, then set world as active.
	 */
	if (!world) {
		return QuickRunActiveWorld;
	}
	else {
		QuickRunActiveWorld = world;
	}
}

/**
 * -----------------------------------------------------------------------------
 * Transform related functions
 * -----------------------------------------------------------------------------
 */

bool entity_set_transform(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot, const DgVec3 scale) {
	C_Transform *trans = entity_find_trans(world, id);
	
	if (!trans) {
		return false;
	}
	
	trans->pos = pos;
	trans->rot = rot;
	trans->scale = scale;
	
	return true;
}

/**
 * -----------------------------------------------------------------------------
 * Player and game management
 * -----------------------------------------------------------------------------
 */

DgVec3 world_get_player_position(const World * const restrict world) {
	DgVec3 pos = DgVec3New(0.0f, 0.0f, 0.0f);
	C_Transform *trans;
	
	for (uint32_t i = 0; i < world->trans_count; i++) {
		if (world->trans[i].base.id == world->player_info.id) {
			pos = world->trans[i].pos;
			break;
		}
	}
	
	return pos;
}

float world_get_player_speed(const World * const restrict world) {
	return world->player_info.speed;
}

bool world_reset_player(World * const restrict world) {
	C_Transform *trans = entity_find_trans(world, world->player_info.id);
	
	if (trans) {
		trans->pos = DgVec3New(0.0f, 2.0f, trans->pos.z + 10.0f);
		return true;
	}
	
	return false;
}

/**
 * -----------------------------------------------------------------------------
 * Pausing and game state
 * -----------------------------------------------------------------------------
 */

void world_set_pause(World * const restrict world, bool paused) {
	world->paused = paused;
}

bool world_get_pause(World * const restrict world) {
	return world->paused;
}

void world_get_speed(const World * const restrict world, float * restrict min, float * restrict max) {
	/**
	 * Get the min and max speed.
	 */
	
	*min = world->game.speed_min;
	*max = world->game.speed_max;
}

void world_set_speed(World * const restrict world, const float min, const float max) {
	/**
	 * Set speed range and map it.
	 */
	
	// Get the old speed
	const float cmin = world->game.speed_min;
	const float cmax = world->game.speed_max;
	
	// Set new speed
	world->game.speed_min = min;
	world->game.speed_max = max;
	
	// Compute player's new speed (scale it)
	const float player_speed = world->player_info.speed;
	const float n = (player_speed - cmin) * (1.0f / (cmax - cmin)); // now in rage [0.0, 1.0]
	
	world->player_info.speed = (n * (max - min)) + min; // make it up to speed again
	
	// check nan
	if ((0x7F800000 & *(int *) &world->player_info.speed) == 0x7F800000) {
		world->player_info.speed = 1.0f;
	}
}

void world_put_length(World * const restrict world, float number) {
	/**
	 * Set speed range and map it.
	 */
	
	world->game.new_length = number;
}

float world_get_level_offset(const World * const restrict world) {
	/**
	 * Get the current level's Z offset.
	 */
	
	return (world->game.load_next/* - world->game.new_length ?*/);
}
