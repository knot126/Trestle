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

typedef uint32_t GameScriptHandle;

typedef struct ScriptManager {
	// List of scripts currently loaded
	bool     *script_allocated;
	DgScript *script;
	size_t    script_count;
	size_t    script_alloc;
} ScriptManager;

void scriptman_init(ScriptManager *this);
void scriptman_free(ScriptManager *this);

GameScriptHandle scriptman_create(ScriptManager *this);
void scriptman_destroy(ScriptManager *this, GameScriptHandle handle);
uint32_t scriptman_load(ScriptManager * restrict this, GameScriptHandle handle, char * const restrict path);
GameScriptHandle scriptman_open(ScriptManager * restrict this, char * const restrict path);

void scriptman_update(ScriptManager *this, float delta);
