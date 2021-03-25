/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Systems Manager
 */

#pragma once

#include "graphics/opengl.h"

typedef struct {
	GraphicsInitInfo graphics;
} SystemStates;

void sys_init(SystemStates *sys);
void sys_destroy(SystemStates *sys);
