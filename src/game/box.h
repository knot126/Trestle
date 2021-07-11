/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Box entity generation
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "global/supervisor.h"
#include "util/maths.h"

Name make_box(Supervisor * const restrict sup, const DgVec3 pos, const DgVec3 size, const DgVec3 colour, const char * const texture);
