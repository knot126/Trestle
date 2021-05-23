/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Jax Scripting Language and Interpreter
 */ 

#if defined(DG_JAX_SCRIPT_ENABLED)

#pragma once

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define DG_JAX_SCRIPT_OPTION_128BIT_VARIABLES

#if defined(DG_JAX_SCRIPT_OPTION_128BIT_VARIABLES) && defined(__GNUC__)
typedef __int128 int128_t;
typedef __float128 float128_t;
#endif

typedef enum DgJaxScriptType {
	JAX_SCRIPT_VOID = 0,
	JAX_SCRIPT_INTEGER = 1,
	JAX_SCRIPT_NUMBER = 2,
	JAX_SCRIPT_BOOLEAN = 3,
	JAX_SCRIPT_STRING = 4,
} DgJaxScriptType;

typedef union DgJaxScriptValue {
#if defined(DG_JAX_SCRIPT_OPTION_128BIT_VARIABLES)
	int128_t integer;
	float128_t number;
#else
	int64_t integer;
	long double number;
#endif
	bool boolean;
	const char *string;
} DgJaxScriptValue;

typedef struct DgJaxScriptSymbol {
	char *name;
	DgJaxScriptType type;
} DgJaxScriptSymbol;

typedef struct DgJaxScript {
	DgJaxScriptSymbol *symbols;
	size_t symbol_count;
} DgJaxScript;

#undef DG_JAX_SCRIPT_OPTION_128BIT_VARIABLES

#endif
