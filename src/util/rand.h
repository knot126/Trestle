/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Random Numbers and Strings
 */

#pragma once

#include <inttypes.h>

uint32_t DgRandInt(void);
float DgRandFloat(void);

#if !defined(DG_NO_LUA)

#include "script.h"

void DgRegisterRandFuncs(DgScript *script);

#endif
