/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Input System
 */

#pragma once

#include <stdbool.h>

#include "../graphics/opengl.h"
#include "../graphics/graphics.h"

void input_init(GraphicsInitInfo info);
void input_update(GraphicsInitInfo info);
bool getKeyPressed(int key);
