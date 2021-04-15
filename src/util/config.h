/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * SimpleConfig File Lodader
 */

#pragma once

#include <stdbool.h>

#include "bag.h"

DgBag *DgConfigLoad(char *path, const bool enable_comments);
void DgConfigFree(DgBag *config);
