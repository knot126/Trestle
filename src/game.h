/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * This file should expose ONLY game_main.
 */

#pragma once

#include "util/bag.h"

extern DgBag *g_gameConfig;

int game_main(int argc, char* argv[]);
