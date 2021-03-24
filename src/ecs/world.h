/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Entity-Component-Systems
 */

#pragma once

#include <inttypes.h>

#include "../ecs/compo.h"

typedef uint32_t EntityComponentMask_t;

typedef struct {
	EntityComponentMask_t *mask;
	
} World;
