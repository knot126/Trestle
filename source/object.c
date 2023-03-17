/**
 * Copyright (C) 2023 Knot126
 * ==========================
 * 
 * Item/Object/Entity/Thing class
 */

#include "util/table.h"

#include "includes.h"

#include "scene.h"

DgError TrObjectInit(TrObject *this) {
	/**
	 * Initialise a game object
	 */
	
	memset(this, 0, sizeof *this);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError TrObjectFree(TrObject *this) {
	/**
	 * Free a game object
	 */
	
	return DG_ERROR_SUCCESSFUL;
}

DgError TrObjectSetShape(TrObject *this, TrAABBShape *shape) {
	/**
	 * Set the shape for the given object
	 */
	
	this->shape = *shape;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError TrObjectGetShape(TrObject *this, TrAABBShape **shape) {
	/**
	 * Get the shape for this object
	 */
	
	shape[0] = &this->shape;
	
	return DG_ERROR_SUCCESSFUL;
}
