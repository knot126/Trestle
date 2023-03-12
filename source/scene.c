/**
 * Copyright (C) 2022 - 2023 Knot126
 * =================================
 * 
 * Scene support data structures and algorithms
 */

#include "util/table.h"
#include "util/log.h"

#include "object.h"

#include "includes.h"

#include "scene.h"

DgError TrSceneInit(TrScene *this) {
	/**
	 * Initialise a new scene
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
	 */
	
	DgTableFree(&this->info);
	
	return DG_ERROR_SUCCESS;
}

DgError TrSceneSetObject(TrScene *this, const char *name, TrObject *object) {
	/**
	 * Add an object to the scene
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

DgError TrSceneGetObject(TrScene *this, const char *name, TrObject **output) {
	/**
	 * Add an object to the scene
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
	 */
	
	DgValue key; DgValueStaticString(&key, name);
	
	return DgTableRemove(&this->objects, &key);
}
