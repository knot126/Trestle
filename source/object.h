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
	DgVec2 base;
	DgVec2 b;
	DgVec4 colour;
} TrAABBShape;

typedef struct TrObject {
	TrAABBShape shape;
} TrObject;
