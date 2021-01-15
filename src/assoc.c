#include <string.h>
#include <stdlib.h>

typedef struct {
	char* key;
	char* value;
} __DgDict_node;

typedef struct {
	__DgDict_node* nodes;
} DgDict;

void DgAddToDict(char* k, char* v) {
	
}
