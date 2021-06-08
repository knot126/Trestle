/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Scripting Support Utilities
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "lua.h"

typedef struct {
	lua_State *state;
} DgScript;

void DgScriptInit(DgScript *script);
bool DgScriptLoad(DgScript *script, char * const path);
void DgScriptCall(DgScript *script, char *name);
void DgScriptFree(DgScript *script);
void DgScriptRegister(DgScript *script, const char * name, int (*function)(DgScript *));
