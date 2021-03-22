/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scenes and Entities
 */

#pragma once

#include <inttypes.h>

#include "../utils/array.h"
#include "../game/components.h"

typedef struct Entity {
	uint64_t id;
	Array compos;
};
