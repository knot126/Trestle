/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * PropertyBag implementation
 */

#include <stdlib.h>

typedef struct __DgBagPair {
	char* key;
	char* value;
} __DgBagPair;

typedef struct DgBag {
	const char** key;
	const char** value;
	size_t size;
} DgBag;

DgBag DgBagInit(void);
void DgBagFree(DgBag* pb);
void DgBagPrint(DgBag* pb);
const char *DgBagGet(DgBag* pb, const char* key);
void DgBagSet(DgBag* pb, const char* key, const char* value);
