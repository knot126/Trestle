/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh representation and tools
 */

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "graphics/graphics.h"
#include "graphics/vertex3d.h"
#include "util/fs.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/rand.h"
#include "util/log.h"
#include "util/str.h"
#include "util/stream.h"
#include "util/obj.h"

#include "mesh.h"

bool graphics_load_xml_mesh(GraphicsSystem * restrict this, Name name, const char * const restrict path) {
	/**
	 * Load and generate an XML format mesh to an entity
	 */
	
// 	// Find the mesh component
// 	Mesh *mesh = graphics_get_mesh(this, name);
// 	
// 	if (!mesh) {
// 		DgLog(DG_LOG_ERROR, "Failed to load xml mesh or model '%s' to entity %d.", path, name);
// 		return false;
// 	}
// 	
// 	// Start loading the XML doucment
// 	DgXMLNode doc;
// 	
// 	uint32_t status = DgXMLLoad(&doc, path);
// 	
// 	if (status) {
// 		DgXMLNodeFree(&doc);
// 		return false;
// 	}
// 	
// 	// Allocate memory for mesh and indexes
// 	DgStream *vertex = DgStreamCreate();
// 	
// 	if (!vertex) {
// 		DgLog(DG_LOG_ERROR, "Failed to load xml mesh or model '%s' to entity %d.", path, name);
// 		DgXMLNodeFree(&doc);
// 		return false;
// 	}
// 	
// 	DgStream *index = DgStreamCreate();
// 	
// 	if (!index) {
// 		DgLog(DG_LOG_ERROR, "Failed to load xml mesh or model '%s' to entity %d.", path, name);
// 		DgXMLNodeFree(&doc);
// 		DgStreamFree(vertex);
// 		return false;
// 	}
// 	
// 	size_t processed_vertex_count = 0;
// 	
// 	// Traverse the XML document and load the data
// 	for (size_t i = 0; i < doc.sub_count; i++) {
// 		if (!strcmp(doc.sub[i].name, "plane") || !strcmp(doc.sub[i].name, "face")) {
// 			DgXMLNode *plane = &doc.sub[i];
// 			
// 			// Prepare the index offsets
// 			size_t index_offsets_size;
// 			uint32_t *index_offsets = DgInt32ListFromString(DgXMLGetAttrib(plane, "indexOffsets", NULL), &index_offsets_size);
// 			
// 			if (index_offsets) {
// 				for (uint32_t j = 0; j < index_offsets_size; j++) {
// 					uint32_t z = processed_vertex_count + index_offsets[j];
// 					DgStreamWriteUInt32(index, &z);
// 				}
// 				
// 				DgFree(index_offsets);
// 			}
// 			
// 			// Loop around the vertexes in the plane
// 			for (size_t j = 0; j < plane->sub_count; j++) {
// 				DgXMLNode *vtxnode = &plane->sub[j];
// 				
// 				if (!strcmp(vtxnode->name, "vertex")) {
// 					DgVec3 pos = DgVec3FromString(DgXMLGetAttrib(vtxnode, "pos", NULL));
// 					DgVec2 texturepos = DgVec2FromString(DgXMLGetAttrib(vtxnode, "texturepos", NULL));
// 					DgVec3 colour = DgVec3FromString(DgXMLGetAttrib(vtxnode, "colour", NULL));
// 					
// 					DgStreamWriteFloat(vertex, &pos.x);
// 					DgStreamWriteFloat(vertex, &pos.y);
// 					DgStreamWriteFloat(vertex, &pos.z);
// 					
// 					DgStreamWriteFloat(vertex, &texturepos.x);
// 					DgStreamWriteFloat(vertex, &texturepos.y);
// 					
// 					DgStreamWriteFloat(vertex, &colour.x);
// 					DgStreamWriteFloat(vertex, &colour.y);
// 					DgStreamWriteFloat(vertex, &colour.z);
// 					
// 					processed_vertex_count += 1;
// 				}
// 			}
// 		}
// 	}
// 	
// 	// Set the mesh component values
// 	float *ptr_vertex;
// 	size_t ptr_vertex_size;
// 	DgBufferFromStream(vertex, (void *) &ptr_vertex, &ptr_vertex_size);
// 	
// 	uint32_t *ptr_index;
// 	size_t    ptr_index_size;
// 	DgBufferFromStream(index, (void *) &ptr_index, &ptr_index_size);
// 	
// 	mesh->vert = ptr_vertex;
// 	mesh->vert_count = ptr_vertex_size / (sizeof(float) * 8);
// 	mesh->index = ptr_index;
// 	mesh->index_count = ptr_index_size / sizeof(uint32_t);
// 	
// 	const char *tn = DgXMLGetAttrib(&doc, "texture", NULL);
// 	if (tn) {
// 		mesh->texture = DgStrdup(tn);
// 	}
// 	
// 	mesh->updated = true;
// 	
// 	// Free XML document
// 	DgXMLNodeFree(&doc);
// 	
// 	return true;
	return false;
}

bool graphics_load_obj_mesh(GraphicsSystem * restrict this, Name name, const char * const restrict path) {
	/**
	 * Load an XML mesh
	 */
	
	// Find the mesh component
	Mesh *mesh = graphics_get_mesh(this, name);
	
	if (!mesh) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh or model '%s' to entity %d: mesh not found.", path, name);
		return false;
	}
	
	DgOBJMesh obj;
	
	uint32_t status = DgOBJLoad(&obj, (char *) path);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh or model '%s' to entity %d: failed to load OBJ.", path, name);
		return false;
	}
	
	QRVertex1 *vert = (QRVertex1 *) DgAlloc(sizeof *vert * obj.vertex_count);
	
	if (!vert) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh or model '%s' to entity %d: failed to allocate memory.", path, name);
		DgOBJFree(&obj);
		return false;
	}
	
	for (size_t i = 0; i < obj.vertex_count; i++) {
		vert[i].x = obj.vertex[i].x;
		vert[i].y = obj.vertex[i].y;
		vert[i].z = obj.vertex[i].z;
		
		vert[i].u = 0.0f;
		vert[i].v = 0.0f;
		
		vert[i].r = DgRandFloat();
		vert[i].g = DgRandFloat();
		vert[i].b = DgRandFloat();
	}
	
	mesh->vert = (float *) vert;
	mesh->vert_count = obj.vertex_count;
	mesh->index = obj.face;
	mesh->index_count = obj.face_count;
	
	mesh->updated = true;
	
	DgFree(obj.vertex);
	
	return true;
}
