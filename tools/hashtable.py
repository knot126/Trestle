KeyType = "const char *" # String type
ValueType = "char *" # Value type
HashType = "uint32_t" # Hash type

StructTypeName = "StringStringTable" # table type name

ValueCompareFunction = "strcmp" # name of strcmp-like function for the key type
HashFunctionName = "DgHashStringU32" # name of hash function

KeyDupFunction = "DgStrdup" # function to duplicate a key given by value
ValueDupFunction = "DgStrdup" # function to duplicate a value given by value

ValueFreeFunction = "DgFree" # function to free a duplicated value given that object by value
KeyFreeFunction = "DgFree" # function to free a key object given that object by value
# If they have no dynamic allocation just leave them blank, they will just make a void expression.

# Allocator name
ReallocateFunction = "DgRealloc"
ExtraIncludes = "#include \"util/hash.h\"\n#include \"util/str.h\"\n#include \"util/alloc.h\""

FileName = "{}".format(StructTypeName)
FileNameC = FileName + ".c"
FileNameH = FileName + ".h"

pre = """/**
 * Automactially generated hash table functions. I tried to make them as 
 * efficent as is possible without using unnessicary memory (e.g. binary trees) 
 * by taking advantage of cache and data locality.
 *
 * == PARAMETERS ==
 * Key-Type: \"{}\"
 * Value-Type: \"{}\"
 * Hash-Type: \"{}\"
 * Type-Name: \"{}\"
 * Compare-Function: \"{}\"
 * Hash-Function: \"{}\"
 * Key-Duplicate-Function: \"{}\"
 * Value-Duplicate-Function: \"{}\"
 * Reallocate-Function: \"{}\"
 * Export-File-Name: \"{}\"
 */

// required includes
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

// extra includes
{}

#include \"{}\"
""".format(KeyType, ValueType, HashType, StructTypeName, ValueCompareFunction, HashFunctionName, KeyDupFunction, ValueDupFunction, ReallocateFunction, FileName, ExtraIncludes, FileNameH)

type1 = """
typedef struct {} {{
	/**
	 * A structure that stores all the information about a hash table. This is
	 * the only structure that this file defines.
	 * 
	 * The hash table is stored using three dense arrays: one for the keys (
	 * which are probably 'const char *'), one for the hashes (probably some
	 * type of integer, cannot be dynamic and must be densely packed for\
	 * preformance reasons) and finally one for the value (which could be a 
	 * reference to some objects via pointers, or could store the objects 
	 * themselves.)
	 * 
	 * It is allowed to specify custom freeing and duplicating functions for 
	 * keys and values when generating the file, so not using any defines.
	 */
	
	{} *key;
	{} *hash;
	{} *value;
	
	size_t count;
	size_t alloc;
}} {};
""".format(StructTypeName, KeyType, HashType, ValueType, StructTypeName)

func_new = """
void {}Init({} * const restrict t) {{
	/**
	 * Initialise the hash table at the given address.
	 */
	
	memset(t, 0, sizeof *t);
}}
""".format(StructTypeName, StructTypeName)

func_alloc = """
static int8_t {}Realloc({} * const restrict t) {{
	/**
	 * Reallocate the hash table if it needs to be reallocated. This should only
	 * be used by internal functions.
	 */
	
	if (t->alloc == t->count) {{
		t->alloc = 2 + (t->alloc * 2);
		
		t->key = {}(t->key, sizeof *t->key * t->alloc);
		if (!t->key) {{ return 1; }}
		
		t->hash = {}(t->hash, sizeof *t->hash * t->alloc);
		if (!t->hash) {{ return 2; }}
		
		t->value = {}(t->value, sizeof *t->value * t->alloc);
		if (!t->value) {{ return 3; }}
	}}
	
	return 0;
}}
""".format(StructTypeName, StructTypeName, ReallocateFunction, ReallocateFunction, ReallocateFunction)

func_set = """
int8_t {}Set({} * const restrict t, {} k, {} v) {{
	/**
	 * Set a key to the given value.
	 */
	
	if ({}Realloc(t)) {{
		return 1;
	}}
	
	size_t index = 0;
	{} want = {}(k);
	char found = 0;
	
	// find item
	for (size_t i = 0; i < t->count; ++i) {{
		if (t->hash[i] == want) {{
			if (!{}(t->key[i], k)) {{
				index = i;
				found = 1;
				break;
			}}
		}}
	}}
	
	// Add to the end of the list if it was not found
	if (!found) {{
		index = t->count;
		t->count++;
	}}
	
	t->key[index] = {}(k);
	t->hash[index] = want;
	t->value[index] = {}(v);
	
	return 0;
}}
""".format(StructTypeName, StructTypeName, KeyType, ValueType, StructTypeName, HashType, HashFunctionName, ValueCompareFunction, KeyDupFunction, ValueDupFunction, KeyDupFunction, ValueDupFunction)

func_get = """
{} *{}Get({} * const restrict t, {} k) {{
	/**
	 * Get a reference to an object in the hash table.
	 */
	
	size_t index = 0;
	{} want = {}(k);
	char found = 0;
	
	// find item
	for (size_t i = 0; i < t->count; ++i) {{
		if (t->hash[i] == want) {{
			if (!{}(t->key[i], k)) {{
				index = i;
				found = 1;
				break;
			}}
		}}
	}}
	
	// replace the current key, value and hash without reallocating if found
	if (found) {{
		return &t->value[index];
	}}
	
	return NULL;
}}
""".format(ValueType, StructTypeName, StructTypeName, KeyType, HashType, HashFunctionName, ValueCompareFunction)

func_getKeyAt = """
{} *{}GetKeyAt({} * const restrict t, size_t index) {{
	/**
	 * Get the key at a certian index. Mainly used for looping over values.
	 */
	
	return &t->key[index];
}}
""".format(KeyType, StructTypeName, StructTypeName)

func_getValueAt = """
{} *{}GetValueAt({} * const restrict t, size_t index) {{
	/**
	 * Get the value at a certian index. Mainly used for looping over values.
	 */
	
	return &t->value[index];
}}
""".format(ValueType, StructTypeName, StructTypeName)

func_getLength = """
size_t {}GetLength({} * const restrict t) {{
	/**
	 * Get the length of the hash table.
	 */
	
	return t->count;
}}
""".format(StructTypeName, StructTypeName)

func_free = """
void {}Free({} * const restrict t) {{
	/**
	 * Free the entire hash table.
	 */
	
	if (t->key) {{
		// Free the keys
		for (size_t i = 0; i < t->count; i++) {{
			(void) {}((void *) t->key[i]);
		}}
		
		t->key = {}(t->key, 0);
	}}
	
	if (t->hash) {{
		t->hash = {}(t->hash, 0);
	}}
	
	if (t->value) {{
		// Free the values
		for (size_t i = 0; i < t->count; i++) {{
			(void) {}(t->value[i]);
		}}
		
		t->value = {}(t->value, 0);
	}}
	
	t->alloc = 0;
	t->count = 0;
}}
""".format(StructTypeName, StructTypeName, KeyFreeFunction, ReallocateFunction, ReallocateFunction, ValueFreeFunction, ReallocateFunction)

Output = pre + func_new + func_alloc + func_set + func_get + func_getKeyAt + func_getValueAt + func_getLength + func_free

f = open(FileNameC, "w")
f.write(Output)
print(Output)

Output = "#pragma once\n" + pre + type1 + """
void {}Init({} * const restrict t);
int8_t {}Set({} * const restrict t, {} k, {} v);
{} *{}Get({} * const restrict t, {} k);
{} *{}GetKeyAt({} * const restrict t, size_t index);
{} *{}GetValueAt({} * const restrict t, size_t index);
size_t {}GetLength({} * const restrict t);
void {}Free({} * const restrict t);
""".format(StructTypeName, StructTypeName,
	StructTypeName, StructTypeName, KeyType, ValueType,
	ValueType, StructTypeName, StructTypeName, KeyType,
	KeyType, StructTypeName, StructTypeName,
	ValueType, StructTypeName, StructTypeName,
	StructTypeName, StructTypeName,
	StructTypeName, StructTypeName)

f = open(FileNameH, "w")
f.write(Output)
print(Output)
