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
	
	if (world->ui) {
		if (world->ui->text) {
			for (uint32_t i = 0; i < world->ui->text_count; i++) {
				world_ui_free_text(&world->ui->text[i]);
			}
			
			DgFree(world->ui->text);
		}
		
		if (world->ui->box) {
			DgFree(world->ui->box);
		}
		
		DgFree(world->ui);
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

uint32_t world_create_ui_element(World * const restrict world, mask_t mask) {
	/**
	 * Create a new entity in the UI subworld and init if it does not exist yet,
	 * returns its element id.
	 */
	
	DgLog(DG_LOG_DEPRECATION, "world_create_ui_element(%X, %d)", world, mask);
	
	// Create the UI world if it does not exist
	if (!world->ui) {
		world->ui = DgAlloc(sizeof(UIWorld));
		
		if (!world->ui) {
			DgLog(DG_LOG_FATAL, "Failed to allocate memory for UI element world.");
			return 0;
		}
		
		memset(world->ui, 0, sizeof(UIWorld));
	}
	
	world->ui->mask_count += 1;
	
	if ((mask & QR_ELEMUI_BOX) == QR_ELEMUI_BOX) {
		world->ui->box_count += 1;
		world->ui->box = DgRealloc(world->ui->box, sizeof(C_UIBox) * world->ui->box_count);
		
		if (!world->ui->box) {
			DgLog(DG_LOG_FATAL, "Failed to allocate memory for UI box element.");
			return 0;
		}
		
		memset(&world->ui->box[world->ui->box_count - 1], 0, sizeof(C_UIBox));
		world->ui->box[world->ui->box_count - 1].base.id = world->ui->mask_count;
	}
	
	if ((mask & QR_ELEMUI_TEXT) == QR_ELEMUI_TEXT) {
		world->ui->text_count += 1;
		world->ui->text = DgRealloc(world->ui->text, sizeof(C_UIText) * world->ui->text_count);
		
		if (!world->ui->text) {
			DgLog(DG_LOG_FATAL, "Failed to allocate memory for UI text element.");
			return 0;
		}
		
		memset(&world->ui->text[world->ui->text_count - 1], 0, sizeof(C_UIText));
		C_UIText *element = &world->ui->text[world->ui->text_count - 1];
		element->base.id = world->ui->mask_count;
		element->size = 0.1f;
		element->pos = DgVec2New(-1.0f, 1.0f);
		element->colour = DgVec4New(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	return world->ui->mask_count;
}

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

#include "world/makebox.h"

void world_set_camera(World * const restrict world, const uint32_t id) {
	/*
	 * Setter function for the world's active camera.
	 * 
	 * Implementation note: We add one to I so that we can check if no camera
	 * has been set and use default matrix in that case.
	 */
	
	for (size_t i = 0; i < world->trans_count; i++) {
		if (world->trans[i].base.id == id) {
			world->cam_active[0] = i + 1;
			break;
		}
	}
	
	for (size_t i = 0; i < world->cam_count; i++) {
		if (world->cam[i].base.id == id) {
			world->cam_active[1] = i + 1;
			break;
		}
	}
	
	world->cam_active[2] = id;
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
 * Physics component related functions
 * -----------------------------------------------------------------------------
 */

bool entity_phys_set_flags(World * const restrict world, const uint32_t id, const int flags) {
	C_Physics *phys = entity_find_phys(world, id);
	
	if (!phys) {
		return false;
	}
	
	phys->flags = flags;
	
	return true;
}

bool entity_phys_set_mass(World * const restrict world, const uint32_t id, const float mass) {
	C_Physics *phys = entity_find_phys(world, id);
	
	if (!phys) {
		return false;
	}
	
	phys->mass = mass;
	
	return true;
}

bool entity_phys_add_force(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot) {
	C_Physics *phys = entity_find_phys(world, id);
	
	if (!phys) {
		return false;
	}
	
	phys->Fpos = DgVec3Scale(phys->mass, pos);
	phys->Frot = DgVec3Scale(phys->mass, rot);
	
	return true;
}

#include "world/uitext.h"

#include "world/uibox.h"

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
