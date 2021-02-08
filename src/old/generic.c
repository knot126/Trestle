/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Old generic type
 */

#include <inttypes.h>

#include "alloc.h"
#include "generic.h"

DgGeneric DgCreateNullGeneric() {
	DgGeneric object;
	
	object.type = DG_DATATYPE_NULL;
	object.data = NULL;
	
	return object;
}

DgGeneric DgCreateInt8Generic(int8_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_INT8;
	object.data = DgAlloc(sizeof(int8_t));
	((int8_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateInt16Generic(int16_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_INT16;
	object.data = DgAlloc(sizeof(int16_t));
	((int16_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateInt32Generic(int32_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_INT32;
	object.data = DgAlloc(sizeof(int32_t));
	((int32_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateInt64Generic(int64_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_INT64;
	object.data = DgAlloc(sizeof(int64_t));
	((int64_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateUInt16Generic(uint16_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_UINT16;
	object.data = DgAlloc(sizeof(uint16_t));
	((uint16_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateUInt32Generic(uint32_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_UINT32;
	object.data = DgAlloc(sizeof(uint32_t));
	((uint32_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateUInt64Generic(uint64_t value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_UINT64;
	object.data = DgAlloc(sizeof(uint64_t));
	((uint64_t *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateFloatGeneric(float value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_FLOAT;
	object.data = DgAlloc(sizeof(float));
	((float *) object.data)[0] = value;
	
	return object;
}

DgGeneric DgCreateDoubleGeneric(double value) {
	DgGeneric object;
	
	object.type = DG_DATATYPE_DOUBLE;
	object.data = DgAlloc(sizeof(double));
	((double *) object.data)[0] = value;
	
	return object;
}


void DgFreeGeneric(DgGeneric* object) {
	DgFree(object->data);
}
