/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Resource and Asset Management Facilities
 */

#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "alloc.h"
#include "str.h"
#include "hash.h"
#include "log.h"

#include "resource.h"

DgResourceManager *DG_defaultResourceManager;

// =============================================================================
// Resource Manager Creation and Destruction
// =============================================================================

DgResourceManager *DgResourceManagerNew(void) {
	/**
	 * Create a new instance of a resource manager.
	 */
	
	DgResourceManager *resman = (DgResourceManager *) DgAlloc(sizeof *resman);
	
	if (!resman) {
		return NULL;
	}
	
	memset(resman, 0, sizeof *resman);
	
	return resman;
}

void DgResourceManagerFree(DgResourceManager *resman) {
	/**
	 * Free a resource manager.
	 */
	
	for (size_t i = 0; i < resman->length; i++) {
		DgResourceInfo a = (DgResourceInfo) {.path = 0, .asset = resman->res[i].res, .mode = DG_RES_FREE};
		(resman->res[i].type)(&a);
		DgFree((void *) resman->res[i].fullkey);
	}
	
	if (resman->keys) {
		DgFree(resman->keys);
	}
	
	if (resman->res) {
		DgFree(resman->res);
	}
	
	DgFree(resman);
}

// =============================================================================
// Get and Set Default Resource Manager
// =============================================================================

void DgResourceManagerSetDefault(DgResourceManager *resman) {
	/**
	 * Set the default resource manager.
	 */
	
	DG_defaultResourceManager = resman;
}

DgResourceManager *DgResourceManagerGetDefault(void) {
	/**
	 * Get the default resource manager.
	 */
	
	return DG_defaultResourceManager;
}

// =============================================================================
// Loading and Unloading Assets
// =============================================================================

void *DgResourceLoad(DgResourceManager *resman, DgResourceFunction load, const char * const path) {
	/**
	 * Load the resource at the given path and return the resource info. It will
	 * need to be cast to the proper return type based on its info.
	 */
	
	// Check to use default resman
	if (!resman) {
		resman = DG_defaultResourceManager;
		if (!resman) {
			return NULL;
		}
	}
	
	// Call the resource type's loading functions
	DgResourceInfo res = (DgResourceInfo) {
		.path = path,
		.asset = NULL,
		.mode = DG_RES_LOAD,
	};
	
	res.asset = (load)(&res);
	
	// Reallocate memory for resman list of resources
	resman->length++;
	
	resman->keys = (uint32_t *) DgRealloc(resman->keys, sizeof *resman->keys * resman->length);
	
	if (!resman->keys) {
		// Return early upon failure
		DgLog(DG_LOG_ERROR, "Memory reallocation for resource manager failed.");
		return res.asset;
	}
	
	resman->res = (DgResource *) DgRealloc(resman->res, sizeof *resman->res * resman->length);
	
	if (!resman->res) {
		// Return early upon failure
		DgFree(resman->keys);
		DgLog(DG_LOG_ERROR, "Memory reallocation for resource manager failed.");
		return res.asset;
	}
	
	// Properly store the asset in the list
	resman->keys[resman->length - 1] = DgHashStringU32(path);
	
	resman->res[resman->length - 1].fullkey = DgStrdup(path);
	resman->res[resman->length - 1].type = load;
	resman->res[resman->length - 1].res = res.asset;
	
	return res.asset;
}

void DgResourceFree(DgResourceManager *resman, const char *name) {
	/**
	 * Free a resource that is normally at the location called by name.
	 */
	
	// Check to use default resman
	if (!resman) {
		resman = DG_defaultResourceManager;
		if (!resman) {
			return;
		}
	}
	
	uint32_t want = DgHashStringU32(name);
	
	for (size_t i = 0; i < resman->length; i++) {
		if (resman->keys[i] == want) {
			// Compare the fullkey strings just to make sure.
			if (!strcmp(resman->res[i].fullkey, name)) {
				DgResourceInfo a = (DgResourceInfo) {
					.path = 0,
					.asset = resman->res[i].res,
					.mode = DG_RES_FREE
				};
				(resman->res[i].type)(&a);
				DgFree((void *) resman->res[i].fullkey);
				break;
			} // strcmp
		} // key[i] == want
	} // for each res
}
