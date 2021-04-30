/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games INI Parser
 */

#include <string.h>

#include "alloc.h"
#include "fs.h"

/*
 * Private Data Types
 */

typedef struct Dg_INIToken {
	enum {
		
	} type;
	char * text;
} Dg_INIToken;
