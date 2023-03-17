/**
 * Copyright (C) 2022 - 2023 Knot126
 * =================================
 * 
 * Scene support data structures and algorithms
 */

#include "util/table.h"
#include "util/log.h"
#include "util/alloc.h"

#include "object.h"

#include "includes.h"

#include "scene.h"

DgError TrSceneInit(TrScene *this) {
	/**
	 * Initialise a new scene
	 * 
	 * @param this Scene object
	 * @return Error code
	 */
	
	memset(this, 0, sizeof *this);
	
	// Initialise info
	DgError status = DgTableInit(&this->info);
	
	if (status) {
		return status;
	}
	
	DgValue key; DgValueStaticString(&key, "running");
	DgValue value; DgValueBool(&value, true);
	
	status = DgTableSet(&this->info, &key, &value);
	
	if (status) {
		return status;
	}
	
	// Initialise item table
	status = DgTableInit(&this->objects);
	
	if (status) {
		return status;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError TrSceneFree(TrScene *this) {
	/**
	 * Free the scene data
	 * 
	 * @param this Scene object
	 * @return Error code
	 */
	
	DgTableFree(&this->info);
	
	return DG_ERROR_SUCCESS;
}

DgError TrSceneSetObject(TrScene *this, const char *name, TrObject *object) {
	/**
	 * Add an object to the scene
	 * 
	 * @param this Scene object
	 * @param name Name of the object to set
	 * @param object The object to set to
	 * @return Error code
	 */
	
	// Find and remove old object if it exists
	DgError status = TrSceneGetObject(this, name, NULL);
	
	if (!status) {
		DgError status = TrSceneRemoveObject(this, name);
		
		if (status) {
			return status;
		}
	}
	
	// Add this object
	DgValue key; DgValueString(&key, name);
	DgValue value; DgValuePointer(&value, (void *) object);
	
	status = DgTableSet(&this->objects, &key, &value);
	
	return status;
}

DgError TrSceneCreateObject(TrScene *this, const char *name, TrObject **object) {
	/**
	 * Create an object given its name. Unlike "set" this will allocate memory
	 * for the object automatically.
	 * 
	 * @param this Scene object
	 * @param name Name of the object to create
	 * @param object The returned object memory
	 * @return Error code
	 */
	
	object[0] = DgMemoryAllocate(sizeof **object);
	
	if (!object[0]) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	DgError status = TrObjectInit(object[0]);
	
	if (status) {
		return status;
	}
	
	return TrSceneSetObject(this, name, object[0]);
}

DgError TrSceneGetObject(TrScene *this, const char *name, TrObject **output) {
	/**
	 * Add an object to the scene
	 * 
	 * @param this Scene object
	 * @param name Name of the object to get
	 * @param object Where to put the pointer to the object
	 * @return Error code
	 */
	
	DgValue searchkey; DgValueStaticString(&searchkey, name);
	DgValue result;
	
	DgError status = DgTableGet(&this->objects, &searchkey, &result);
	
	if (!status && output) {
		output[0] = result.data.asPointer;
	}
	
	return (result.type == DG_TABLE_TYPE_POINTER) ? status : DG_ERROR_FAILED;
}

DgError TrSceneRemoveObject(TrScene *this, const char *name) {
	/**
	 * Add an object to the scene
	 * 
	 * @param this Scene object
	 * @param name Name of the object to remove
	 * @return Error code
	 */
	
	DgValue key; DgValueStaticString(&key, name);
	DgValue value;
	
	DgError status = DgTableGet(&this->objects, &key, &value);
	
	if (status) {
		return status;
	}
	
	DgFree(value.data.asPointer);
	
	DgValueStaticString(&key, name);
	
	return DgTableRemove(&this->objects, &key);
}

DgError TrSceneAtObject(TrScene *this, size_t index, TrObject **object) {
	/**
	 * Get the object at the given index
	 * 
	 * @note Error is DG_ERROR_NOT_FOUND if beyond the array
	 * 
	 * @param this Scene object
	 * @param index Index of the object
	 * @param object Where to put the object
	 * @return Error code
	 */
	
	DgValue value;
	
	DgError status = DgTableAt(&this->objects, index, NULL, &value);
	
	if (status) {
		return status;
	}
	
	object[0] = value.data.asPointer;
	
	return DG_ERROR_SUCCESS;
}
