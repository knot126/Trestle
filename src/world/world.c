/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "compo.h"
#include "../util/fs.h"
#include "../util/alloc.h"
#include "../util/fail.h"
#include "../util/xml.h"
#include "../util/maths.h"
#include "../util/log.h"

#include "world.h"

World *QuickRunActiveWorld;

void world_init(World *world, size_t prealloc_count) {
	/*
	 * Creates a world full of entities
	 */
	memset(world, 0, sizeof(World));
	
	if (prealloc_count == 0) {
		return;
	}
	
	// prealloc is disabled for now...
}

void world_destroy(World *world) {
	/* 
	 * Frees the a world and its exsiting resources
	 */
	
	if (world->mask) {
		DgFree(world->mask);
	}
	
	if (world->CTransforms) {
		DgFree(world->CTransforms);
	}
	
	if (world->CMeshs) {
		DgFree(world->CMeshs);
	}
	
	if (world->CCameras) {
		DgFree(world->CCameras);
	}
	
	if (world->ui) {
		if (world->ui->text) {
			DgFree(world->ui->text);
		}
		
		if (world->ui->box) {
			DgFree(world->ui->box);
		}
		
		DgFree(world->ui);
	}
}

uint32_t world_create_entity(World *world, mask_t mask) {
	/*
	 * Create a new entity in a given world and return its ID
	 */
	world->mask_count++;
	
	// NOTE: nullptr passed to realloc works like alloc, so we do not need to
	// consider a case where the list has not been initialised.
	world->mask = DgRealloc(world->mask, sizeof(mask_t) * world->mask_count);
	
	if (!world->mask) {
		DgFail("Allocation error: world->mask.\n", 403);
	}
	
	world->mask[world->mask_count - 1] = mask;
	
	// Allocate the nessicary components
	
	// Transform
	if ((mask & QR_COMPONENT_TRANSFORM) == QR_COMPONENT_TRANSFORM) {
		world->CTransforms_count++;
		world->CTransforms = DgRealloc(world->CTransforms, sizeof(CTransform) * world->CTransforms_count);
		
		if (!world->CTransforms) {
			DgFail("Allocation error: world->CTransforms.\n", 400);
		}
		
		memset((world->CTransforms + (world->CTransforms_count - 1)), 0, sizeof(CTransform));
		world->CTransforms[world->CTransforms_count - 1].base.id = world->mask_count;
	}
	
	// Mesh
	if ((mask & QR_COMPONENT_MESH) == QR_COMPONENT_MESH) {
		world->CMeshs_count++;
		world->CMeshs = DgRealloc(world->CMeshs, sizeof(CMesh) * world->CMeshs_count);
		
		if (!world->CMeshs) {
			DgFail("Allocation error: world->CMeshs.\n", 401);
		}
		
		memset((world->CMeshs + (world->CMeshs_count - 1)), 0, sizeof(CMesh));
		world->CMeshs[world->CMeshs_count - 1].base.id = world->mask_count;
	}
	
	// Camera
	if ((mask & QR_COMPONENT_CAMERA) == QR_COMPONENT_CAMERA) {
		world->CCameras_count++;
		world->CCameras = DgRealloc(world->CCameras, sizeof(CCamera) * world->CCameras_count);
		
		if (!world->CCameras) {
			DgFail("Allocation error: world->CCameras\n", 402);
		}
		
		memset((world->CCameras + (world->CCameras_count - 1)), 0, sizeof(CCamera));
		world->CCameras[world->CCameras_count - 1].base.id = world->mask_count;
	}
	
	// Physics
	if ((mask & QR_COMPONENT_PHYSICS) == QR_COMPONENT_PHYSICS) {
		world->CPhysicss_count++;
		world->CPhysicss = DgRealloc(world->CPhysicss, sizeof(CPhysics) * world->CPhysicss_count);
		
		if (!world->CPhysicss) {
			DgFail("Allocation error: world->CPhysicss\n", 402);
		}
		
		memset((world->CPhysicss + (world->CPhysicss_count - 1)), 0, sizeof(CPhysics));
		world->CPhysicss[world->CPhysicss_count - 1].base.id = world->mask_count;
	}
	
	return world->mask_count;
}

uint32_t world_create_ui_element(World * const restrict world, mask_t mask) {
	/**
	 * Create a new entity in the UI subworld and init if it does not exist yet,
	 * returns its element id.
	 */
	world->mask_count += 1;
	
	// Reallocate the masks list
	world->mask = DgRealloc(world->mask, sizeof(mask_t) * world->mask_count);
	
	if (!world->mask) {
		DgFail("Allocation error: world->mask.\n", 403);
	}
	
	world->mask[world->mask_count - 1] = mask;
	
	// Create the UI world if it does not exist
	if (!world->ui) {
		world->ui = DgAlloc(sizeof(UIWorld));
		
		if (!world->ui) {
			DgLog(DG_LOG_FATAL, "Failed to allocate memory for UI element world.");
			return 0;
		}
		
		memset(world->ui, 0, sizeof(UIWorld));
	}
	
	if ((mask & QR_ELEMUI_BOX) == QR_ELEMUI_BOX) {
		world->ui->box_count += 1;
		world->ui->box = DgRealloc(world->ui->box, sizeof(C_UIBox) * world->ui->box_count);
		
		if (!world->ui->box) {
			DgLog(DG_LOG_FATAL, "Failed to allocate memory for UI box element.");
			return 0;
		}
		
		memset(&world->ui->box[world->ui->box_count - 1], 0, sizeof(C_UIBox));
		world->ui->box[world->ui->box_count - 1].base.id = world->mask_count;
	}
	
	if ((mask & QR_ELEMUI_TEXT) == QR_ELEMUI_TEXT) {
		world->ui->text_count += 1;
		world->ui->text = DgRealloc(world->ui->text, sizeof(C_UIText) * world->ui->text_count);
		
		if (!world->ui->text) {
			DgLog(DG_LOG_FATAL, "Failed to allocate memory for UI text element.");
			return 0;
		}
		
		memset(&world->ui->text[world->ui->text_count - 1], 0, sizeof(C_UIText));
		world->ui->text[world->ui->text_count - 1].base.id = world->mask_count;
	}
	
	return world->mask_count;
}

void SetActiveWorld(World *world) {
	QuickRunActiveWorld = world;
}

bool entity_load_mesh(World * const restrict world, uint32_t id, char * const restrict path) {
	/*
	 * Loads an uncompressed mesh from a file into an entity's mesh component
	 */
	
	// Find the mesh component
	CMesh *mesh = NULL;
	
	for (int i = 0; i < world->CMeshs_count; i++) {
		if (world->CMeshs[i].base.id == id) {
			mesh = &world->CMeshs[i];
			break;
		}
	}
	
	if (!mesh) {
		printf("Failed to load mesh or model '%s' to entity %d.\n", path, id);
		return false;
	}
	
	// Open the file stream
	char *real_path = DgEvalPath(path);
	DgFileStream *s = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!s) {
		printf("Failed to load file '%s'.\n", path);
		return false;
	}
	
	// Read vertexes
	DgFileStreamReadInt32(s, &mesh->vert_count);
	mesh->vert = DgAlloc(mesh->vert_count * 32);
	if (!mesh->vert) {
		printf("Failed to allocate memory whilst trying to load a mesh file.\n");
		return false;
	}
	DgFileStreamRead(s, mesh->vert_count * 32, mesh->vert);
	
	// Read indexes
	DgFileStreamReadInt32(s, &mesh->index_count);
	mesh->index = DgAlloc(mesh->index_count * 4);
	if (!mesh->index) {
		printf("Failed to allocate memory whilst trying to load a mesh file.\n");
		return false;
	}
	DgFileStreamRead(s, mesh->index_count * 4, mesh->index);
	
	mesh->updated = true;
	
	DgFileStreamClose(s);
	
	return true;
}

bool entity_load_xmesh(World * const restrict world, uint32_t id, const char * const restrict path) {
	/**
	 * Load and generate an XML format mesh to an entity
	 */
	DgXMLNode doc;
	
	uint32_t status = DgXMLLoad(&doc, path);
	
	if (status) {
		return false;
	}
	
	// Calculate size needed to be allocated
	// Using: Number of planes * Number of verticies per plane * Number of floats per vertex
	float *data = (float *) DgAlloc(doc.sub_count * 4 * 8);
	
	if (!data) {
		return false;
	}
	
	for (size_t i = 0; i < doc.sub_count; i++) {
		if (!strcmp(doc.sub[i].name, "plane")) {
			DgVec3 pos = DgVec3FromString(DgXMLGetAttrib(&doc, "pos", NULL));
			DgVec3 size = DgVec3FromString(DgXMLGetAttrib(&doc, "size", NULL));
			DgVec3 colour = DgVec3FromString(DgXMLGetAttrib(&doc, "colour", NULL));
			
			data[((i + 0) * 32) + 0] = pos.x + size.x;
			data[((i + 0) * 32) + 1] = pos.y + size.y;
			data[((i + 0) * 32) + 2] = pos.z + size.z;
			
			// TODO: Finish this, I am going to work on something else now ...
		}
	}
	
	DgXMLNodeFree(&doc);
}

void world_set_camera(World * const restrict world, const uint32_t id) {
	/*
	 * Setter function for the world's active camera.
	 * 
	 * Implementation note: We add one to I so that we can check if no camera
	 * has been set and use default matrix in that case.
	 */
	for (size_t i = 0; i < world->CTransforms_count; i++) {
		if (world->CTransforms[i].base.id == id) {
			world->CCameras_active[0] = i + 1;
			break;
		}
	}
	
	for (size_t i = 0; i < world->CCameras_count; i++) {
		if (world->CCameras[i].base.id == id) {
			world->CCameras_active[1] = i + 1;
			break;
		}
	}
	
	world->CCameras_active[2] = id;
}


bool entity_set_transform(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot, const DgVec3 scale) {
	CTransform *trans = NULL;
	
	// Find the transfrom
	for (uint32_t i = 0; i < world->CTransforms_count; i++) {
		if (world->CTransforms[i].base.id == id) {
			trans = (world->CTransforms + i);
			break;
		}
	}
	
	if (!trans) {
		return false;
	}
	
	trans->pos = pos;
	trans->rot = rot;
	trans->scale = scale;
	
	return true;
}

bool entity_phys_set_flags(World * const restrict world, const uint32_t id, const int flags) {
	CPhysics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == id) {
			phys = (world->CPhysicss + i);
			break;
		}
	}
	
	if (!phys) {
		return false;
	}
	
	phys->flags = flags;
	
	return true;
}

bool entity_phys_set_mass(World * const restrict world, const uint32_t id, const float mass) {
	CPhysics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == id) {
			phys = (world->CPhysicss + i);
			break;
		}
	}
	
	if (!phys) {
		return false;
	}
	
	phys->mass = mass;
	
	return true;
}

bool entity_phys_add_force(World * const restrict world, const uint32_t id, const DgVec3 pos, const DgVec3 rot) {
	CPhysics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->CPhysicss_count; i++) {
		if (world->CPhysicss[i].base.id == id) {
			phys = (world->CPhysicss + i);
			break;
		}
	}
	
	if (!phys) {
		return false;
	}
	
	phys->Fpos = DgVec3Scale(phys->mass, pos);
	phys->Frot = DgVec3Scale(phys->mass, rot);
	
	return true;
}

