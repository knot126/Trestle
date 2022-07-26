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

typedef struct {
	void *context;
	
	int32_t (*init)(InputSystem * const restrict this, void * const restrict context);
	int32_t (*update)(InputSystem * const restrict this, void * const restrict context);
	void (*free)(InputSystem * const restrict this, void * const restrict context);
} trInputBackend;

void input_init(InputSystem *input, GraphicsSystem *graphics);
void input_update(InputSystem *input);

bool getKeyPressed(int key);
DgVec2 getMousePos(void);
DgVec2 getMouseDelta(void);
