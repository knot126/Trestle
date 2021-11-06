/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System
 */

#pragma once

#include <stdbool.h>

#include "graphics/graphics.h"

#include <GLFW/glfw3.h>

typedef struct {
	GraphicsSystem *graphics;
} InputSystem;

void input_init(InputSystem *input, GraphicsSystem *graphics);
void input_update(InputSystem *input);

bool getKeyPressed(int key);
DgVec2 getMousePos(void);
DgVec2 getMouseDelta(void);
