/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Jax Scripting Language and Interpreter
 */

#define DG_JAX_SCRIPT_ENABLED

#if defined(DG_JAX_SCRIPT_ENABLED)

#include <string.h>

#include "alloc.h"
#include "str.h"

#include "script_jax.h"

DgJaxScript *DgJaxScriptNew(void) {
	DgJaxScript *script = DgAlloc(sizeof(DgJaxScript));
	
	if (!script) {
		return NULL;
	}
	
	memset(script, 0, sizeof(DgJaxScript));
	
	
	
	return script;
}

void DgJaxScriptParse(DgJaxScript *script) {
	/**
	 * Load a script text into script memory
	 */
}

#endif
