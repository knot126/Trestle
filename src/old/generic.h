#include <inttypes.h>

typedef enum DgType {
	DG_DATATYPE_NULL     = 0,  // Null object type
	DG_DATATYPE_INT8     = 1,  //   8-bit Integer
	DG_DATATYPE_INT16    = 2,  //  16-bit Integer
	DG_DATATYPE_UINT16   = 3,  //  16-bit Unsigned Integer
	DG_DATATYPE_INT32    = 4,  //  32-bit Integer
	DG_DATATYPE_UINT32   = 5,  //  32-bit Unsigned Integer
	DG_DATATYPE_INT64    = 6,  //  64-bit Integer
	DG_DATATYPE_UINT64   = 7,  //  64-bit Unsigned Integer
	DG_DATATYPE_INTN     = 8,  //   N-bit Integer
	DG_DATATYPE_FLOAT    = 9,  // Float
	DG_DATATYPE_DOUBLE   = 10, // Double
	DG_DATATYPE_REAL     = 11, // Long Double
	DG_DATATYPE_CHAR     = 12, // Char
	DG_DATATYPE_BYTESEQ  = 13, // Byte sequence/array container
	DG_DATATYPE_STRING   = 14, // Pointer to null-terminated string
	DG_DATATYPE_ARRAY    = 15, // Pointer to Generic array container
	DG_DATATYPE_OBJECT   = 16, // Assocative array container
} DgType;

typedef struct DgGeneric {
	DgType    type;
	void     *data;
} DgGeneric;

typedef struct DgGenericObject {
	uint64_t  type;
	void     *data;
} DgGenericObject;
