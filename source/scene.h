/**
 * Copyright (C) 2022 - 2023 Knot126
 * =================================
 * 
 * Scene support data structures and algorithms
 */

#pragma once

#include "util/table.h"
#include "util/error.h"

#include "object.h"

#include "includes.h"

typedef struct {
	const char *name;
	DgTable info;
	DgTable objects;
} TrScene;

DgError TrSceneInit(TrScene *this);
DgError TrSceneFree(TrScene *this);

DgError TrSceneSetObject(TrScene *this, const char *name, TrObject *object);
DgError TrSceneGetObject(TrScene *this, const char *name, TrObject **output);
DgError TrSceneRemoveObject(TrScene *this, const char *name);
