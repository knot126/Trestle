/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Rectangle entity generation
 */

#include <stdbool.h>
#include <inttypes.h>

#include "global/supervisor.h"
#include "util/maths.h"

Name make_rect(Supervisor * const restrict sup, const DgVec2 pos, const DgVec2 size, const DgVec4 colour);
