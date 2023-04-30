/**
 * Copyright (C) 2023 Knot126
 * ==========================
 * 
 * Item/Object/Entity/Thing class
 */

#pragma once

#include "util/table.h"
#include "util/error.h"
#include "util/maths.h"

#include "includes.h"

typedef struct TrAABBShape {
	DgVec3 position;
	DgVec3 size;
	DgVec4 colour;
} TrAABBShape;

typedef struct TrObject {
	TrAABBShape shape;
} TrObject;

DgError TrObjectInit(TrObject *this);
DgError TrObjectFree(TrObject *this);
DgError TrObjectSetShape(TrObject *this, TrAABBShape *shape);
DgError TrObjectGetShape(TrObject *this, TrAABBShape **shape);
