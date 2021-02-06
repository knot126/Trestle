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
char *DgPropertyBagGet(DgPropertyBag pb, char* name);
void DgPropertyBagSet(DgPropertyBag pb, char* key, char* value);
