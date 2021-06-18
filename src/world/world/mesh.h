/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh
 */

#pragma once

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
	C_Mesh *mesh = entity_find_mesh(world, id);
	
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
	C_Mesh *mesh = entity_find_mesh(world, id);
	
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
	
	mesh->updated = true;
	
	// Free XML document
	DgXMLNodeFree(&doc);
	
	return true;
}
