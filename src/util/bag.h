#include <stdlib.h>

typedef struct DgPropertyBagPair {
	char* key;
	char* value;
} DgPropertyBagPair;

typedef struct DgPropertyBag {
	DgPropertyBagPair* pairs;
	size_t size;
} DgPropertyBag;

DgPropertyBag DgPropertyBagInit(size_t size);
