/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GUI and Widget Library
 */

#include <inttypes.h>
#include <stdbool.h>

#include "alloc.h"
#include "maths.h"

#include "ui.h"

DgUISurface *DgUISurfaceNew(void) {
	/**
	 * Create a new UI surface (like a window).
	 */
	
	DgUISurface *surface = (DgUISurface *) DgAlloc(sizeof *surface);
	
	if (!surface) {
		return NULL;
	}
	
	memset(surface, 0, sizeof *surface);
	
	return surface;
}

void DgUISurfaceFree(DgUISurface *surface) {
	/**
	 * Free an exsiting UI surface.
	 */
	
	DgFree(surface);
}

void DgUISurfaceSetDefaultBase(DgUISurface *surface, DgUIElementBase *base) {
	/**
	 * Set default base parameters.
	 */
	
	surface->defaultBase = *base;
}

void DgUISurfaceSetDefaultFontMetrics(DgUISurface *surface, DgUIFontMetrics *metrics) {
	/**
	 * Set default font metrics.
	 */
	
	surface->defaultFont = *metrics;
}

void DgUISurfaceSetDefaultPanelMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics) {
	/**
	 * Set default panel metrics.
	 */
	
	surface->defaultPanel = *metrics;
}
