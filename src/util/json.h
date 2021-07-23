/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Simple JSON Parser
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum DgJSONValueType {
	DG_JSON_INTEGER,     // 12345
	DG_JSON_NUMBER,      // 123.45
	DG_JSON_STRING,      // "abc"
	DG_JSON_ARRAY,       // ["a", 1, 2, 3]
	DG_JSON_OBJECT,      // {"a": "b", "c": 34}
	DG_JSON_BOOLEAN,     // false | true
	DG_JSON_NULL,        // null
} DgJSONValueType;

struct DgJSONValue;

typedef struct DgJSONArray {
	/**
	 * A JSON array of values.
	 * 
	 * Ex: [5, "a", 17.5, null, false]
	 */
	
	struct DgJSONValue *data;
	size_t count;
	size_t alloc;
} DgJSONArray;

typedef struct DgJSONObject {
	/**
	 * A JSON object, basically a hash table.
	 * 
	 * Ex: {"key": "value", "a": 643, "def": false}
	 */
	
	const char **key;
	uint32_t *hash;
	struct DgJSONValue *value;
	size_t count;
	size_t alloc;
} DgJSONObject;

typedef union DgJSONValueData {
	/**
	 * The data for a value, without storing the value's type.
	 */
	
	int64_t as_integer;
	double as_number;
	const char *as_string;
	DgJSONArray *as_array;
	DgJSONObject *as_object;
	bool as_boolean;
} DgJSONValueData;

typedef struct DgJSONValue {
	/**
	 * A value's data stored with it's type.
	 */
	
	DgJSONValueType type;
	DgJSONValueData value;
} DgJSONValue;

int32_t DgJSONParse(DgJSONValue * restrict doc, const size_t length, const char * const restrict source);

void DgJSONValueFree(DgJSONValue * restrict value);
void DgJSONValuePrint(DgJSONValue * restrict value);

DgJSONArray *DgJSONArrayInit(void);
int32_t DgJSONArrayPush(DgJSONArray *array, DgJSONValue value);

DgJSONObject *DgJSONObjectInit(void);
int32_t DgJSONObjectPush(DgJSONObject *obj, const char *key, DgJSONValue value);
void DgJSONObjectFree(DgJSONObject *obj);
