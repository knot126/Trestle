/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Global State Manager
 */

#pragma once

typedef struct {
	float speed_min;
	float speed_max;
	
	float load_next;
	float new_length;
} GameState;
