/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GUI and Widget Library
 */

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "alloc.h"
#include "maths.h"
#include "str.h"

#include "ui.h"

/**
 * =============================================================================
 * SURFACE CREATE/DESTROY
 * =============================================================================
 */

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

/**
 * =============================================================================
 * CONSTRUCTORS
 * =============================================================================
 */

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

/**
 * =============================================================================
 * CONSTRUCTORS
 * =============================================================================
 */

DgUIElementBase DgUIElementBaseNew(DgVec2 pos, bool visible, bool enable) {
	/**
	 * Create DgUIElementBase object
	 */
	
	DgUIElementBase obj;
	
	obj.pos = pos;
	obj.visible = visible;
	obj.enable = enable;
	
	return obj;
}

DgUIFontMetrics DgUIFontMetricsNew(DgVec4 colour, float size, int32_t font, uint32_t align) {
	/**
	 * Create DgUIFontMetrics object
	 */
	
	DgUIFontMetrics obj;
	
	obj.colour = colour;
	obj.size = size;
	obj.font = font;
	obj.align = align;
	
	return obj;
}

DgUIPanelMetrics DgUIPanelMetricsNew(DgVec4 colour, DgVec4 padding, DgVec2 size, float radius) {
	/**
	 * Create DgUIPanelMetrics object
	 */
	
	DgUIPanelMetrics obj;
	
	obj.colour = colour;
	obj.padding = padding;
	obj.size = size;
	obj.radius = radius;
	
	return obj;
}

DgUIActionSpec DgUIActionSpecNew(void *context, void (*callback)(void *context)) {
	/**
	 * Create DgUIActionSpec object
	 */
	
	DgUIActionSpec obj;
	
	obj.context = context;
	obj.callback = callback;
	
	return obj;
}

DgUIPanel DgUIPanelNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIPanelMetrics *panel) 
{
	/**
	 * Create a new panel
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUIPanel obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.panel = (panel) ? *panel : surface->defaultPanel;
	
	return obj;
}

DgUILabel DgUILabelNew(
	DgUISurface *surface, 
	DgUIElementBase *base,
	DgUIFontMetrics *font, 
	const char * const content) 
{
	/**
	 * Create a new label
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUILabel obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.font = (font) ? *font : surface->defaultFont;
	
	obj.content = DgStrdup(content);
	obj.length = strlen(obj.content);
	
	return obj;
}

DgUILineEdit DgUILineEditNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	const char * const placeholder, 
	const char * const content, 
	DgUIPanelMetrics *s_style)
{
	/**
	 * Create a new lineedit
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUILineEdit obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.font = (font) ? *font : surface->defaultFont;
	obj.panel = (panel) ? *panel : surface->defaultPanel;
	
	obj.placeholder = DgStrdup(placeholder);
	obj.placeholder_length = strlen(obj.placeholder);
	
	obj.content = DgStrdup(content);
	obj.length = strlen(obj.content);
	obj.curpos = obj.length;
	
	obj.s_style = (s_style) ? *s_style : surface->defaultPanel;
	
	return obj;
}

DgUIButton DgUIButtonNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	const char * const content,
	DgUIActionSpec *action)
{
	/**
	 * Create a new button
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUIButton obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.font = (font) ? *font : surface->defaultFont;
	obj.panel = (panel) ? *panel : surface->defaultPanel;
	
	obj.content = DgStrdup(content);
	obj.length = strlen(obj.content);
	
	obj.action = (action) ? *action : (DgUIActionSpec) {.context = NULL, .callback = NULL};
	obj.pressed = false;
	
	return obj;
}

/**
 * =============================================================================
 * DESTRUCTORS
 * =============================================================================
 */

static void DgUILineEditFree(DgUILineEdit *obj) {
	DgFree((void *) obj->placeholder);
	DgFree((void *) obj->content);
}

static void DgUIButtonFree(DgUIButton *obj) {
	DgFree((void *) obj->content);
}

/**
 * =============================================================================
 * ADD/REMOVE COMPONENTS
 * =============================================================================
 */

#define DG_UI_ADD_ELEMENT(FUNCNAME, TYPENAME, ACCESS_NAME, COUNT_NAME) size_t FUNCNAME(DgUISurface *surface, TYPENAME obj) {\
	surface->ACCESS_NAME = (TYPENAME *) DgRealloc(surface->ACCESS_NAME, sizeof *surface->ACCESS_NAME * ++surface->COUNT_NAME);\
	\
	if (!surface->ACCESS_NAME) {\
		return 0;\
	}\
	\
	surface->ACCESS_NAME[surface->COUNT_NAME - 1] = obj;\
	\
	return surface->COUNT_NAME;\
}

#define DG_UI_REMOVE_ELEMENT(FUNCNAME, TYPENAME, ACCESS_NAME, COUNT_NAME) size_t FUNCNAME(DgUISurface *surface, size_t ident) {\
	size_t index = ident - 1;\
	\
	if (surface->COUNT_NAME - ident > 0) {\
		memmove(&surface->ACCESS_NAME[index], &surface->ACCESS_NAME[ident], surface->COUNT_NAME - ident);\
	}\
	\
	surface->ACCESS_NAME = (TYPENAME *) DgRealloc(surface->ACCESS_NAME, sizeof *surface->ACCESS_NAME * --surface->COUNT_NAME);\
	\
	if (!surface->ACCESS_NAME) {\
		return 0;\
	}\
	\
	return ident;\
}

DG_UI_ADD_ELEMENT(DgUISurfaceAddPanel, DgUIPanel, panel, panel_count);
DG_UI_ADD_ELEMENT(DgUISurfaceAddLabel, DgUILabel, label, label_count);
DG_UI_ADD_ELEMENT(DgUISurfaceAddLineEdit, DgUILineEdit, lineedit, lineedit_count);
DG_UI_ADD_ELEMENT(DgUISurfaceAddButton, DgUIButton, button, button_count);

DG_UI_REMOVE_ELEMENT(DgUISurfaceRemovePanel, DgUIPanel, panel, panel_count);
DG_UI_REMOVE_ELEMENT(DgUISurfaceRemoveLabel, DgUILabel, label, label_count);
DG_UI_REMOVE_ELEMENT(DgUISurfaceRemoveLineEdit, DgUILineEdit, lineedit, lineedit_count);
DG_UI_REMOVE_ELEMENT(DgUISurfaceRemoveButton, DgUIButton, button, button_count);

#undef DG_UI_ADD_ELEMENT
#undef DG_UI_REMOVE_ELEMENT

/**
 * =============================================================================
 * UPDATE ROUTINES
 * =============================================================================
 */

void DgUILineEditUpdateText(DgUILineEdit *object, unsigned keycode) {
	/**
	 * Update the LineEdit object to add or remove letters, or move the cursor
	 * using the arrow keys.
	 */
	
	
}

void DgUISurfaceUpdate(DgUISurface *surface, DgUISurfaceUpdateStructure *info) {
	/**
	 * Call all of the needed update routines.
	 */
	
}

void DgUISurfaceRenderData(DgUISurface *surface, DgUIRenderData *data) {
	/**
	 * Generate and return the render data.
	 */
}
