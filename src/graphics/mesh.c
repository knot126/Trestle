/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Mesh representation and tools
 * 
 * @deprecated This is not needed
 */

#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "graphics/graphics.h"
#include "graphics/vertex3d.h"
#include "util/fs.h"
#include "util/alloc.h"
#include "util/maths.h"
#include "util/rand.h"
#include "util/log.h"
#include "util/str.h"
#include "util/stream.h"

#include "mesh.h"

bool graphics_load_xml_mesh(GraphicsSystem * restrict this, Name name, const char * const restrict path) {
	/**
	 * Load and generate an XML format mesh to an entity
	 * 
	 * @deprecated Does not work
	 */
	
	return false;
}

bool graphics_load_obj_mesh(GraphicsSystem * restrict this, Name name, const char * const restrict path) {
	/**
	 * Load an OBJ mesh file
	 * 
	 * @deprecated Not needed
	 */
	
	return false;
}
