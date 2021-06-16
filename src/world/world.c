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
#include "../util/str.h"
#include "../util/stream.h"

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
	
	if (world->mask) {
		DgFree(world->mask);
	}
	
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
	world->mask = DgRealloc(world->mask, sizeof(mask_t) * world->mask_count);
	
	if (!world->mask) {
		DgFail("Allocation error: world->mask.\n", 403);
	}
	
	world->mask[world->mask_count - 1] = mask;
	
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
	
	// Reallocate the masks list
	world->ui->mask = DgRealloc(world->ui->mask, sizeof(mask_t) * world->ui->mask_count);
	
	if (!world->ui->mask) {
		DgFail("Allocation error: world->ui->mask.\n", 403);
	}
	
	world->ui->mask[world->ui->mask_count - 1] = mask;
	
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
	}
	
	return world->ui->mask_count;
}

/**
 * -----------------------------------------------------------------------------
 * Helper functions
 * -----------------------------------------------------------------------------
 */

C_Mesh *entity_find_mesh(const World * const restrict world, uint32_t id) {
	C_Mesh *mesh = NULL;
	
	for (int i = 0; i < world->mesh_count; i++) {
		if (world->mesh[i].base.id == id) {
			mesh = &world->mesh[i];
			break;
		}
	}
	
	return mesh;
}

/**
 * -----------------------------------------------------------------------------
 * Active world setters and getters
 * -----------------------------------------------------------------------------
 */

void SetActiveWorld(World *world) {
	QuickRunActiveWorld = world;
}

/**
 * -----------------------------------------------------------------------------
 * Mesh utilities
 * -----------------------------------------------------------------------------
 */

bool entity_load_mesh(World * const restrict world, uint32_t id, char * const restrict path) {
	/*
	 * Loads an uncompressed mesh from a file into an entity's mesh component
	 */
	
	// Find the mesh component
	C_Mesh *mesh = NULL;
	
	for (int i = 0; i < world->mesh_count; i++) {
		if (world->mesh[i].base.id == id) {
			mesh = &world->mesh[i];
			break;
		}
	}
	
	if (!mesh) {
		DgLog(DG_LOG_ERROR, "Failed to load mesh or model '%s' to entity %d.", path, id);
		return false;
	}
	
	// Open the file stream
	char *real_path = DgEvalPath(path);
	DgFileStream *s = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!s) {
		DgLog(DG_LOG_ERROR, "Failed to load file '%s'.", path);
		return false;
	}
	
	// Read vertexes
	DgFileStreamReadInt32(s, &mesh->vert_count);
	mesh->vert = DgAlloc(mesh->vert_count * 32);
	if (!mesh->vert) {
		DgFileStreamClose(s);
		DgLog(DG_LOG_ERROR, "Failed to allocate memory whilst trying to load a mesh file.");
		return false;
	}
	DgFileStreamRead(s, mesh->vert_count * 32, mesh->vert);
	
	// Read indexes
	DgFileStreamReadInt32(s, &mesh->index_count);
	mesh->index = DgAlloc(mesh->index_count * 4);
	if (!mesh->index) {
		DgFileStreamClose(s);
		DgFree(mesh->vert);
		DgLog(DG_LOG_ERROR, "Failed to allocate memory whilst trying to load a mesh file.");
		return false;
	}
	DgFileStreamRead(s, mesh->index_count * 4, mesh->index);
	
	mesh->updated = true;
	
	DgFileStreamClose(s);
	
	return true;
}

bool entity_load_xml_mesh(World * const restrict world, uint32_t id, const char * const restrict path) {
	/**
	 * Load and generate an XML format mesh to an entity
	 */
	
	// Find the mesh component
	C_Mesh *mesh = NULL;
	
	for (int i = 0; i < world->mesh_count; i++) {
		if (world->mesh[i].base.id == id) {
			mesh = &world->mesh[i];
			break;
		}
	}
	
	if (!mesh) {
		DgLog(DG_LOG_ERROR, "Failed to load xml mesh or model '%s' to entity %d.", path, id);
		return false;
	}
	
	// Start loading the XML doucment
	DgXMLNode doc;
	
	uint32_t status = DgXMLLoad(&doc, path);
	
	if (status) {
		DgXMLNodeFree(&doc);
		return false;
	}
	
	// Allocate memory for mesh and indexes
	DgStream *vertex = DgStreamCreate();
	
	if (!vertex) {
		DgLog(DG_LOG_ERROR, "Failed to load xml mesh or model '%s' to entity %d.", path, id);
		DgXMLNodeFree(&doc);
		return false;
	}
	
	DgStream *index = DgStreamCreate();
	
	if (!index) {
		DgLog(DG_LOG_ERROR, "Failed to load xml mesh or model '%s' to entity %d.", path, id);
		DgXMLNodeFree(&doc);
		DgStreamFree(vertex);
		return false;
	}
	
	size_t processed_vertex_count = 0;
	
	// Traverse the XML document and load the data
	for (size_t i = 0; i < doc.sub_count; i++) {
		if (!strcmp(doc.sub[i].name, "plane") || !strcmp(doc.sub[i].name, "face")) {
			DgXMLNode *plane = &doc.sub[i];
			
			// Prepare the index offsets
			size_t index_offsets_size;
			uint32_t *index_offsets = DgInt32ListFromString(DgXMLGetAttrib(plane, "indexOffsets", NULL), &index_offsets_size);
			
			if (index_offsets) {
				for (uint32_t j = 0; j < index_offsets_size; j++) {
					uint32_t z = processed_vertex_count + index_offsets[j];
					DgStreamWriteUInt32(index, &z);
				}
				
				DgFree(index_offsets);
			}
			
			// Loop around the vertexes in the plane
			for (size_t j = 0; j < plane->sub_count; j++) {
				DgXMLNode *vtxnode = &plane->sub[j];
				
				if (!strcmp(vtxnode->name, "vertex")) {
					DgVec3 pos = DgVec3FromString(DgXMLGetAttrib(vtxnode, "pos", NULL));
					DgVec2 texturepos = DgVec2FromString(DgXMLGetAttrib(vtxnode, "texturepos", NULL));
					DgVec3 colour = DgVec3FromString(DgXMLGetAttrib(vtxnode, "colour", NULL));
					
					DgStreamWriteFloat(vertex, &pos.x);
					DgStreamWriteFloat(vertex, &pos.y);
					DgStreamWriteFloat(vertex, &pos.z);
					
					DgStreamWriteFloat(vertex, &texturepos.x);
					DgStreamWriteFloat(vertex, &texturepos.y);
					
					DgStreamWriteFloat(vertex, &colour.x);
					DgStreamWriteFloat(vertex, &colour.y);
					DgStreamWriteFloat(vertex, &colour.z);
					
					processed_vertex_count += 1;
				}
			}
		}
	}
	
	// Set the mesh component values
	float *ptr_vertex;
	size_t ptr_vertex_size;
	DgBufferFromStream(vertex, (void *) &ptr_vertex, &ptr_vertex_size);
	
	uint32_t *ptr_index;
	size_t    ptr_index_size;
	DgBufferFromStream(index, (void *) &ptr_index, &ptr_index_size);
	
	mesh->vert = ptr_vertex;
	mesh->vert_count = ptr_vertex_size / (sizeof(float) * 8);
	mesh->index = ptr_index;
	mesh->index_count = ptr_index_size / sizeof(uint32_t);
	
	const char *tn = DgXMLGetAttrib(&doc, "texture", NULL);
	if (tn) {
		mesh->texture = DgStrdup(tn);
	}
	
#if 0
	DgLog(DG_LOG_VERBOSE, "Got %d verticies and %d indexes.", mesh->vert_count, mesh->index_count);
	
	if (!mesh->vert || !mesh->index) {
		DgLog(DG_LOG_ERROR, "Index or vert ptr is null");
	}
	
	for (int i = 0; i < mesh->vert_count; i++) {
		DgLog(DG_LOG_VERBOSE, "V: pos = (%.3f, %.3f, %.3f)", mesh->vert[(i * 8) + 0], mesh->vert[(i * 8) + 1], mesh->vert[(i * 8) + 2]);
	}
	
	for (int i = 0; i < mesh->index_count; i++) {
		DgLog(DG_LOG_VERBOSE, "I: %d", mesh->index[i]);
	}
#endif
	
	mesh->updated = true;
	
	// Free XML document
	DgXMLNodeFree(&doc);
	
	return true;
}

/**
 * -----------------------------------------------------------------------------
 * Higher level entity generation
 * -----------------------------------------------------------------------------
 */

bool entity_generate_box(World * const restrict world, const DgVec3 pos, const DgVec3 size, const DgVec3 colour, const char * const texture) {
	uint32_t ent = world_create_entity(world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH);
	
	if (ent == 0) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", ent);
		return false;
	}
	
	// Find the mesh component
	C_Mesh *mesh = entity_find_mesh(world, ent);
	
	if (!mesh) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", ent);
		return false;
	}
	
	// Set the transform
	if (!entity_set_transform(world, ent, pos, DgVec3New(0.0f, 0.0f, 0.0f), size)) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", ent);
		return false;
	}
	
	// Generate box mesh
	const float s_BoxData[] = {
		// X      Y      Z     U     V     R     G     B
		// Front and Back
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		 
		-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.85f, 0.85f, 0.85f,
		 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.85f, 0.85f, 0.85f,
		
		// Top and bottom
		-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		 
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.95f, 0.95f, 0.95f,
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		 
		// Left and right
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.75f, 0.75f, 0.75f,
		-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.75f, 0.75f, 0.75f,
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.75f, 0.75f, 0.75f,
	};
	
	const uint32_t s_IndexData[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};
	
	float *vertex = (float *) DgAlloc(sizeof(s_BoxData));
	
	if (!vertex) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", ent);
		return false;
	}
	
	memcpy(vertex, s_BoxData, sizeof(s_BoxData));
	
	uint32_t *index = (uint32_t *) DgAlloc(sizeof(s_IndexData));
	
	if (!index) {
		DgLog(DG_LOG_ERROR, "Failed to find mesh for entity %d.", ent);
		DgFree(vertex);
		return false;
	}
	
	memcpy(index, s_IndexData, sizeof(s_IndexData));
	
	// Set mesh data in mesh component
	mesh->vert = vertex;
	mesh->vert_count = 24;
	
	mesh->index = index;
	mesh->index_count = sizeof(s_IndexData) / sizeof(float);
	
	mesh->updated = true;
	
	// Update vertex data
	for (uint32_t i = 0; i < 24; i++) {
		vertex[(i * 8) + 5] = colour.x;
		vertex[(i * 8) + 6] = colour.y;
		vertex[(i * 8) + 7] = colour.z;
	}
	
	// Front and back
	vertex[(0 * 8) + 3] *= size.x;
	vertex[(0 * 8) + 4] *= size.y;
	vertex[(1 * 8) + 3] *= size.x;
	vertex[(1 * 8) + 4] *= size.y;
	vertex[(2 * 8) + 3] *= size.x;
	vertex[(2 * 8) + 4] *= size.y;
	vertex[(3 * 8) + 3] *= size.x;
	vertex[(3 * 8) + 4] *= size.y;
	
	vertex[(4 * 8) + 3] *= size.x;
	vertex[(4 * 8) + 4] *= size.y;
	vertex[(5 * 8) + 3] *= size.x;
	vertex[(5 * 8) + 4] *= size.y;
	vertex[(6 * 8) + 3] *= size.x;
	vertex[(6 * 8) + 4] *= size.y;
	vertex[(7 * 8) + 3] *= size.x;
	vertex[(7 * 8) + 4] *= size.y;
	
	// Top and bottom
	vertex[(8 * 8) + 3] *= size.x;
	vertex[(8 * 8) + 4] *= size.z;
	vertex[(9 * 8) + 3] *= size.x;
	vertex[(9 * 8) + 4] *= size.z;
	vertex[(10 * 8) + 3] *= size.x;
	vertex[(10 * 8) + 4] *= size.z;
	vertex[(11 * 8) + 3] *= size.x;
	vertex[(11 * 8) + 4] *= size.z;
	
	vertex[(12 * 8) + 3] *= size.x;
	vertex[(12 * 8) + 4] *= size.z;
	vertex[(13 * 8) + 3] *= size.x;
	vertex[(13 * 8) + 4] *= size.z;
	vertex[(14 * 8) + 3] *= size.x;
	vertex[(14 * 8) + 4] *= size.z;
	vertex[(15 * 8) + 3] *= size.x;
	vertex[(15 * 8) + 4] *= size.z;
	
	// Sides
	vertex[(16 * 8) + 3] *= size.y;
	vertex[(16 * 8) + 4] *= size.z;
	vertex[(17 * 8) + 3] *= size.y;
	vertex[(17 * 8) + 4] *= size.z;
	vertex[(18 * 8) + 3] *= size.y;
	vertex[(18 * 8) + 4] *= size.z;
	vertex[(19 * 8) + 3] *= size.y;
	vertex[(19 * 8) + 4] *= size.z;
	
	vertex[(20 * 8) + 3] *= size.y;
	vertex[(20 * 8) + 4] *= size.z;
	vertex[(21 * 8) + 3] *= size.y;
	vertex[(21 * 8) + 4] *= size.z;
	vertex[(22 * 8) + 3] *= size.y;
	vertex[(22 * 8) + 4] *= size.z;
	vertex[(23 * 8) + 3] *= size.y;
	vertex[(23 * 8) + 4] *= size.z;
	
	// Texture
	if (texture) {
		mesh->texture = DgStrdup(texture);
	}
	
	return true;
}

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
	C_Transform *trans = NULL;
	
	// Find the transfrom
	for (uint32_t i = 0; i < world->trans_count; i++) {
		if (world->trans[i].base.id == id) {
			trans = (world->trans + i);
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

/**
 * -----------------------------------------------------------------------------
 * Physics component related functions
 * -----------------------------------------------------------------------------
 */

bool entity_phys_set_flags(World * const restrict world, const uint32_t id, const int flags) {
	C_Physics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->phys_count; i++) {
		if (world->phys[i].base.id == id) {
			phys = (world->phys + i);
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
	C_Physics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->phys_count; i++) {
		if (world->phys[i].base.id == id) {
			phys = (world->phys + i);
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
	C_Physics *phys = NULL;
	
	// Find it
	for (uint32_t i = 0; i < world->phys_count; i++) {
		if (world->phys[i].base.id == id) {
			phys = (world->phys + i);
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

/**
 * -----------------------------------------------------------------------------
 * UI Text
 * -----------------------------------------------------------------------------
 */

bool ui_element_set_text(World * const restrict world, const uint32_t id, const char * const restrict text) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		return false;
	}
	
	if (element->text) {
		DgFree((char *) element->text);
	}
	
	element->text = DgStrdup((char *) text);
	
	if (!element->text) {
		return false;
	}
	
	element->updated = true;
	
	return true;
}

bool ui_element_set_text_pos(World * const restrict world, const uint32_t id, const DgVec2 pos) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		DgLog(DG_LOG_ERROR, "Failed to find text for UI element: %d", id);
		return false;
	}
	
	element->pos = pos;
	
	element->updated = true;
	
	return true;
}

bool ui_element_set_text_size(World * const restrict world, const uint32_t id, const float size) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		DgLog(DG_LOG_ERROR, "Failed to find text for UI element: %d", id);
		return false;
	}
	
	element->size = size;
	
	element->updated = true;
	
	return true;
}

bool ui_element_set_text_font(World * const restrict world, const uint32_t id, const char * const restrict font) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		DgLog(DG_LOG_ERROR, "Failed to find text for UI element: %d", id);
		return false;
	}
	
	if (element->font) {
		DgFree(element->font);
	}
	
	element->font = DgStrdup(font);
	
	// NOTE:
	// We don't need to set the updated feild for fonts, since that can be applied
	// without changing vertex data...
	
	return true;
}

/**
 * -----------------------------------------------------------------------------
 * Player and game management
 * -----------------------------------------------------------------------------
 */

DgVec3 world_get_player_position(World * const restrict world) {
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

bool world_reset_player(World * const restrict world) {
	C_Transform *trans;
	
	for (uint32_t i = 0; i < world->trans_count; i++) {
		if (world->trans[i].base.id == world->player_info.id) {
			world->trans[i].pos = DgVec3New(0.0f, 2.0f, world->trans[i].pos.z + 10.0f);
			return true;
		}
	}
	
	return false;
}

void world_set_pause(World * const restrict world, bool paused) {
	world->paused = paused;
}
