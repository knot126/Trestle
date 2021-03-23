/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game Context
 */

#pragma once

/* 
 * Game Context Structure
 * 
 * This structure holds information that will be passed to the main loop after
 * initialisation as well as keeping track of all the scenes and the scene data.
 */

// forward declarations
struct DgOpenGLContext;

typedef struct GameContext {
	Array scenes;
	Scene *current;
} GameContext;
