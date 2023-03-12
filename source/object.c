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
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError TrObjectFree(TrObject *this) {
	/**
	 * Free a game object
	 */
	
	memset(this, 0, sizeof *this);
	
	return DG_ERROR_NOT_IMPLEMENTED;
}
