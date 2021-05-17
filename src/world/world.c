/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * World Graph Manager
 */

#include <string.h>

#include "../world/world.h"
#include "../util/fs.h"
#include "../util/alloc.h"
#include "../util/fail.h"
#include "../util/xml.h"
#include "../graphics/opengl_1.h"

#include "world.h"

World *QuickRunActiveWorld;

void world_init(World *world, size_t __UNUSED__) {
	/*
	 * Creates a world full of entities
	 */
	memset(world, 0, sizeof(World));
}

void world_destroy(World *world) {
	/* 
	 * Frees the a world and its exsiting resources
	 */
	
	// Free entity masks
	if (world->ent.trans) {
		DgFree(world->ent.trans);
	}
	
	if (world->ent.mesh) {
		DgFree(world->ent.mesh);
	}
	
	if (world->ent.camera) {
		DgFree(world->ent.camera);
	}
	
	if (world->ent.phys) {
		DgFree(world->ent.phys);
	}
	
	// Free the actual component data
	if (world->trans) {
		DgFree(world->trans);
	}
	
	if (world->mesh) {
		DgFree(world->mesh);
	}
	
	if (world->camera) {
		DgFree(world->camera);
	}
	
	if (world->phys) {
		DgFree(world->phys);
	}
}

Entity world_create_entity(World *world, uint32_t flags) {
	/*
	 * Create a new entity in a given world and return (its index + 1)
	 */
	world->ent_count++;
	
	// Allocate the list entites and their component indicies
	{
		world->ent.trans = DgRealloc(world->ent.trans, world->ent_count * sizeof(Entity));
		world->ent.mesh = DgRealloc(world->ent.mesh, world->ent_count * sizeof(Entity));
		world->ent.camera = DgRealloc(world->ent.camera, world->ent_count * sizeof(Entity));
		world->ent.phys = DgRealloc(world->ent.phys, world->ent_count * sizeof(Entity));
		
		if (!world->ent.trans || !world->ent.mesh || !world->ent.camera || !world->ent.phys) {
			printf("\033[1;31mError:\033[0m Failed to allocate world entity indicies. You might get a segfault.\n");
			return 0;
		}
		
		world->ent.trans[world->ent_count - 1] = -1;
		world->ent.mesh[world->ent_count - 1] = -1;
		world->ent.camera[world->ent_count - 1] = -1;
		world->ent.phys[world->ent_count - 1] = -1;
	}
	
	// Transform
	if ((flags & QR_COMPONENT_TRANSFORM) == QR_COMPONENT_TRANSFORM) {
		world->trans_count = world->trans_count + 1;
		world->trans = DgRealloc(world->trans, sizeof(C_Transform) * world->trans_count);
		
		if (!world->trans) {
			printf("\033[1;31mError:\033[0m Failed to allocate world transform component.\n");
			return 0;
		}
		
		memset(&world->trans[world->trans_count - 1], 0, sizeof(C_Transform));
		world->ent.trans[world->ent_count - 1] = world->trans_count - 1;
	}
	
	// Mesh
	if ((flags & QR_COMPONENT_MESH) == QR_COMPONENT_MESH) {
		world->mesh_count = world->mesh_count + 1;
		world->mesh = DgRealloc(world->mesh, sizeof(C_Mesh) * world->mesh_count);
		
		if (!world->mesh) {
			printf("\033[1;31mError:\033[0m Failed to allocate world mesh component.\n");
			return 0;
		}
		
		memset(&world->mesh[world->mesh_count - 1], 0, sizeof(C_Mesh));
		world->ent.mesh[world->ent_count - 1] = world->mesh_count - 1;
	}
	
	// Camera
	if ((flags & QR_COMPONENT_CAMERA) == QR_COMPONENT_CAMERA) {
		world->camera_count = world->camera_count + 1;
		world->camera = DgRealloc(world->camera, sizeof(C_Camera) * world->camera_count);
		
		if (!world->camera) {
			printf("\033[1;31mError:\033[0m Failed to allocate world camera component.\n");
			return 0;
		}
		
		memset(&world->camera[world->camera_count - 1], 0, sizeof(C_Camera));
		world->ent.camera[world->ent_count - 1] = world->camera_count - 1;
	}
	
	// Physics
	if ((flags & QR_COMPONENT_PHYSICS) == QR_COMPONENT_PHYSICS) {
		world->phys_count = world->phys_count + 1;
		world->phys = DgRealloc(world->phys, sizeof(C_Physics) * world->phys_count);
		
		if (!world->phys) {
			printf("\033[1;31mError:\033[0m Failed to allocate world phys component.\n");
			return 0;
		}
		
		memset(&world->phys[world->phys_count - 1], 0, sizeof(C_Physics));
		world->ent.phys[world->ent_count - 1] = world->phys_count - 1;
	}
	
	return world->ent_count;
}

void SetActiveWorld(World *world) {
	QuickRunActiveWorld = world;
}

bool entity_load_mesh(World * const restrict world, const Entity id, char * restrict path) {
	/*
	 * Loads an uncompressed mesh from a file into an entity's mesh component
	 */
	
// 	printf("Loading mesh file %s...\n", path);
	
	// Find the mesh component
	if (world->ent.mesh[id - 1] < 0) {
		printf("Failed to load mesh or model '%s' to entity %d.\n", path, id);
		return false;
	}
	
	C_Mesh *mesh = &world->mesh[world->ent.mesh[id - 1]];
	
	// Open the file stream
	char *real_path = DgEvalPath(path);
	DgFileStream *s = DgFileStreamOpen(real_path, "rb");
	DgFree(real_path);
	
	if (!s) {
		printf("Failed to load file '%s' whilst trying to load a mesh file.\n", path);
		return false;
	}
	
	// Read vertexes
	DgFileStreamReadInt32(s, &mesh->vert_count);
	mesh->vert = DgAlloc(mesh->vert_count * sizeof(MeshVertex));
	if (!mesh->vert) {
		printf("Failed to allocate memory whilst trying to load a mesh file.\n");
		return false;
	}
	DgFileStreamRead(s, mesh->vert_count * sizeof(MeshVertex), mesh->vert);
	
	// Read indexes
	DgFileStreamReadInt32(s, &mesh->index_count);
	mesh->index = DgAlloc(mesh->index_count * sizeof(uint32_t));
	if (!mesh->index) {
		printf("Failed to allocate memory whilst trying to load a mesh file.\n");
		return false;
	}
	DgFileStreamRead(s, mesh->index_count * sizeof(uint32_t), mesh->index);
	
	mesh->updated = true;
	
	DgFileStreamClose(s);
	
	return true;
}

bool entity_generate_mesh_from_xml(World * const restrict world, const Entity id, const char * const restrict path) {
	return false;
}

void world_set_camera(World * const restrict world, const Entity id) {
	/*
	 * Setter function for the world's active camera.
	 * 
	 * Implementation note: We add one to I so that we can check if no camera
	 * has been set and use default matrix in that case.
	 */
	
	world->camera_active = id;
}

bool entity_set_transform(World * const restrict world, const Entity id, const DgVec3 pos, const DgVec3 rot, const DgVec3 scale) {
	if (world->ent.trans[id - 1] < 0) {
		printf("No transform component for entity %d.\n", id);
		return false;
	}
	
	C_Transform *trans = &world->trans[world->ent.trans[id - 1]];
	
	if (!trans) {
		return false;
	}
	
	trans->pos = pos;
	trans->rot = rot;
	trans->scale = scale;
	
	return true;
}

bool entity_phys_set_flags(World * const restrict world, const Entity id, const int flags) {
	if (world->ent.phys[id - 1] < 0) {
		printf("No physics component for entity %d.\n", id);
		return false;
	}
	
	C_Physics *phys = &world->phys[world->ent.phys[id - 1]];
	
	phys->flags = flags;
	
	return true;
}

bool entity_phys_set_mass(World * const restrict world, const Entity id, float mass) {
	if (world->ent.phys[id - 1] < 0) {
		printf("No physics component for entity %d.\n", id);
		return false;
	}
	
	C_Physics *phys = &world->phys[world->ent.phys[id - 1]];
	
	phys->mass = mass;
	
	return true;
}

bool entity_phys_add_force(World * const restrict world, const Entity id, const DgVec3 pos, const DgVec3 rot) {
	if (world->ent.phys[id - 1] < 0) {
		printf("No physics component for entity %d.\n", id);
		return false;
	}
	
	C_Physics *phys = &world->phys[world->ent.phys[id - 1]];
	
	phys->Fpos = DgVec3Scale(phys->mass, pos);
	phys->Frot = DgVec3Scale(phys->mass, rot);
	
	return true;
}
