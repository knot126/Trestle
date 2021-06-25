/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Resource and Asset Management Facilities
 */ 

#pragma once

#include <inttypes.h>
#include <stdbool.h>

// Modes
enum {
	DG_RES_NONE = 0,
	DG_RES_LOAD,
	DG_RES_SERIALISE,
	DG_RES_FREE,
};

/**
 * The structure that will be passed to the resource type's management function.
 * 
 * If mode is DG_RES_LOAD, then path is a string to the path of the resource
 * (either on the file system or a URI/URL) and asset is NULL. This should
 * return the asset structure.
 * 
 * If mode is DG_RES_SERIALISE, then path is a string to the path where the
 * object needs to be saved and asset is the pointer to the asset. If the asset
 * is serialised okay, then return (void *)1. If an asset cannot be serialised, 
 * then return NULL.
 * 
 * If mode is DG_RES_FREE, then path is NULL and asset is a pointer to the
 * structure to be freed. The return value will be discarded.
 * 
 * If mode is anything else, then the behaviour is not defined.
 */
typedef struct {
	const char *path;
	void *asset;
	uint32_t mode;
} DgResourceInfo;

/**
 * The function that handles an asset should return a (void *) to that asset's
 * structure.
 */
typedef void *(*DgResourceFunction)(DgResourceInfo *) ;

/**
 * A single resource in the list of resources.
 */
typedef struct DgResource {
	const char *fullkey;      // The full key's name, used to resolve ambiguity.
	DgResourceFunction type;  // The type of resource, stored as its function.
	void *res;                // Pointer to the resource info.
} DgResource;

/**
 * A full resource management subsystem object.
 */
typedef struct DgResourceManager {
	uint32_t *keys;        // The key hashes of the resources.
	DgResource *res;       // A list of all the resources.
	size_t length;         // The number of resource that have been loaded.
} DgResourceManager;

DgResourceManager *DgResourceManagerNew(void);
void DgResourceManagerFree(DgResourceManager *resman);

void DgResourceManagerSetDefault(DgResourceManager *resman);
DgResourceManager *DgResourceManagerGetDefault(void);

void *DgResourceLoad(DgResourceManager *resman, DgResourceFunction load, const char * const path);
void DgResourceFree(DgResourceManager *resman, const char *name);
