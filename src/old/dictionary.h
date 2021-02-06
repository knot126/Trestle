#include "generic.h"

typedef struct DgDictionaryPair {
	DgGeneric key;
	DgGeneric value;
} DgDictionaryPair;

typedef struct DgDictionary {
	DgDictionaryPair *pairs;
	size_t size;
}
