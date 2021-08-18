#pragma once
/**
 * Automactially generated hash table functions. I tried to make them as 
 * efficent as is possible without using unnessicary memory (e.g. binary trees) 
 * by taking advantage of cache and data locality.
 *
 * == PARAMETERS ==
 * Key-Type: "const char *"
 * Value-Type: "char *"
 * Hash-Type: "uint32_t"
 * Type-Name: "StringStringTable"
 * Compare-Function: "strcmp"
 * Hash-Function: "DgHashStringU32"
 * Key-Duplicate-Function: "DgStrdup"
 * Value-Duplicate-Function: "DgStrdup"
 * Reallocate-Function: "DgRealloc"
 * Export-File-Name: "StringStringTable"
 */

// required includes
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

// extra includes
#include "util/hash.h"
#include "util/str.h"
#include "util/alloc.h"

#include "StringStringTable.h"

typedef struct StringStringTable {
	/**
	 * A structure that stores all the information about a hash table. This is
	 * the only structure that this file defines.
	 * 
	 * The hash table is stored using three dense arrays: one for the keys (
	 * which are probably 'const char *'), one for the hashes (probably some
	 * type of integer, cannot be dynamic and must be densely packed for	 * preformance reasons) and finally one for the value (which could be a 
	 * reference to some objects via pointers, or could store the objects 
	 * themselves.)
	 * 
	 * It is allowed to specify custom freeing and duplicating functions for 
	 * keys and values when generating the file, so not using any defines.
	 */
	
	const char * *key;
	uint32_t *hash;
	char * *value;
	
	size_t count;
	size_t alloc;
} StringStringTable;

void StringStringTableInit(StringStringTable * const restrict t);
int8_t StringStringTableSet(StringStringTable * const restrict t, const char * k, char * v);
char * *StringStringTableGet(StringStringTable * const restrict t, const char * k);
const char * *StringStringTableGetKeyAt(StringStringTable * const restrict t, size_t index);
char * *StringStringTableGetValueAt(StringStringTable * const restrict t, size_t index);
size_t StringStringTableGetLength(StringStringTable * const restrict t);
void StringStringTableFree(StringStringTable * const restrict t);
