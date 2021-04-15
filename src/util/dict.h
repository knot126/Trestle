/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Dictionary - Hopefully the last implementation!
 */

#pragma once

#include <inttypes.h>

typedef enum {
	DG_DICT_INT = 1,
	DG_DICT_UINT = 2,
	DG_DICT_FLOAT = 3,
	DG_DICT_DOUBLE = 4,
	DG_DICT_STRING = 5,
	DG_DICT_ARRAY = 6,
	DG_DICT_DICT = 7,
} DgDictType;

typedef struct {
	DgDictType type;
	size_t length;
	void *data;
} DgDictArray;

struct DgDict; // needed for DgDictItem

typedef struct DgDictItem {
	DgDictType type;
	union {
		uint64_t asUint;
		int64_t asInt;
		float asFloat;
		double asDouble;
		char *asString;
		DgDictArray *asArray;
		struct DgDict *asDict;
		void *asPtr;
	} data;
} DgDictItem;

typedef struct {
	DgDictItem key;
	DgDictItem value;
} DgDictPair;

typedef struct DgDict {
	DgDictPair *data;
	size_t length;
} DgDict;

DgDict DgDictNew(void);
DgDict *DgDictNew2(void);
void DgDictSet(DgDict *dict, DgDictItem *key, DgDictItem *value);
DgDictItem* DgDictGet(DgDict *dict, DgDictItem *key);
