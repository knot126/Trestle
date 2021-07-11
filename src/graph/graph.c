/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scene Graph
 */

#include <string.h>

#include "util/maths.h"
#include "util/alloc.h"

#include "graph.h"

void graph_init(SceneGraph * restrict this) {
	/**
	 * Initialise a scene graph.
	 */
	
	memset(this, 0, sizeof *this);
}

void graph_free(SceneGraph * restrict this) {
	/**
	 * Free a scene graph.
	 */
	
	if (this->name) {
		DgFree(this->name);
	}
	
	if (this->data) {
		DgFree(this->data);
	}
}

static int graph_alloc(SceneGraph * restrict this) {
	/**
	 * Handle memory mangement.
	 */
	
	if (this->alloc == this->length) {
		this->alloc = 2 + this->alloc * 2;
		
		this->name = DgRealloc(this->name, sizeof *this->name * this->alloc);
		
		if (!this->name) {
			return 1;
		}
		
		this->data = DgRealloc(this->data, sizeof *this->data * this->alloc);
		
		if (!this->data) {
			return 2;
		}
	}
	
	return 0;
}

static size_t graph_find(SceneGraph * restrict this, Name name) {
	/**
	 * Find the index that has a name.
	 */
	
	for (size_t i = 0; i < this->length; i++) {
		if (this->name[i] == name) {
			return i;
		}
	}
	
	return -1;
}

Name graph_create(SceneGraph * restrict this, Name name) {
	/**
	 * Create an entity in the scene graph. Returns a non-zero value on success.
	 */
	
	if (graph_alloc(this)) {
		return 0;
	}
	
	this->name[this->length++] = name;
	
	return name;
}

Name graph_set(SceneGraph * restrict this, Name name, Transform trans) {
	/**
	 * Set a transfrom given a name.
	 */
	
	size_t index = graph_find(this, name);
	
	if (index == -1) {
		return 0;
	}
	
	this->data[index] = trans;
	
	return name;
}

Transform * const graph_get(SceneGraph * restrict this, Name name) {
	/**
	 * Get a transfrom by a name.
	 */
	
	size_t index = graph_find(this, name);
	
	if (index == -1) {
		return NULL;
	}
	
	return &this->data[index];
}
