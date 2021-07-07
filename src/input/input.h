/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System
 */

#pragma once

#include <stdbool.h>

#include "graphics/opengl.h"
#include "graphics/graphics.h"

typedef struct {
	GraphicsSystem *graphics;
} InputSystem;

void input_init(InputSystem *input, GraphicsSystem *graphics);
void input_update(InputSystem *input);
bool getKeyPressed(int key);
