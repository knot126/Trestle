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
	DgScript *script;
	size_t    script_count;
	size_t    script_alloc;
} ScriptManager;

void scriptman_new(ScriptManager *this);
void scriptman_free(ScriptManager *this);

void scriptman_update(ScriptManager *this, float delta);

void scriptman_script_functions(DgScript *script);
