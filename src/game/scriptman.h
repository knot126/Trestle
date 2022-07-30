/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Game Script Manager
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "util/script.h"

typedef int32_t GameScriptHandle;

typedef struct trScriptManager {
	// List of scripts currently loaded
	bool     *script_allocated;
	DgScript *script;
	size_t    script_count;
	size_t    script_alloc;
} trScriptManager;

typedef trScriptManager ScriptManager;

int32_t trScriptManagerInit(ScriptManager *this);
void trScriptManagerFree(ScriptManager *this);

DgScript *trScriptManagerGetScript(ScriptManager *this, GameScriptHandle handle);
GameScriptHandle trScriptManagerAddScript(ScriptManager *this);
int32_t trScriptManagerRemoveScript(ScriptManager *this, GameScriptHandle handle);
int32_t trScriptManagerLoadCode(ScriptManager * restrict this, GameScriptHandle handle, char * const restrict path);
GameScriptHandle trScriptManagerLoadScript(ScriptManager * restrict this, char * const restrict path);

int32_t trScriptManagerTick(ScriptManager *this, float delta);

#define scriptman_init trScriptManagerInit
#define scriptman_free trScriptManagerFree
#define scriptman_create trScriptManagerAddScript
#define scriptman_destroy trScriptManagerRemoveScript
#define scriptman_load trScriptManagerLoadCode
#define scriptman_open trScriptManagerLoadScript
#define scriptman_update trScriptManagerTick
