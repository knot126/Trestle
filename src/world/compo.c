/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Component Freeing Functions
 */

#include "util/alloc.h"

#include "compo.h"

void world_free_mesh_component(C_Mesh * const restrict mesh) {
	if (mesh->vert) {
		DgFree(mesh->vert);
	}
	
	if (mesh->index) {
		DgFree(mesh->index);
	}
}

void world_ui_free_text(C_UIText * const restrict text) {
	if (text->text) {
		DgFree((void *) text->text);
	}
	
	if (text->vertex) {
		DgFree(text->vertex);
	}
	
	if (text->index) {
		DgFree(text->index);
	}
	
	if (text->font) {
		DgFree(text->font);
	}
}
