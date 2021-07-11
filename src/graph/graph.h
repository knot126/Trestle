/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scene Graph - Transform
 */

#pragma once

#include <inttypes.h>

#include "util/maths.h"
#include "types.h"

typedef struct Transform {
	DgVec3 pos;
	DgVec3 rot;
	DgVec3 scale;
} Transform;

typedef struct SceneGraph {
	Name *name;
	Transform *data;
	size_t alloc;
	size_t length;
} SceneGraph;

void graph_init(SceneGraph * restrict this);
void graph_free(SceneGraph * restrict this);
Name graph_create(SceneGraph * restrict this, Name name);
Name graph_set(SceneGraph * restrict this, Name name, Transform trans);
Transform * const graph_get(SceneGraph * restrict this, Name name);
