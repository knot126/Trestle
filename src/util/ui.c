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
	 * Set default font metrics. This will make an interal copy of the structure.
	 */
	
	if (!surface->defaultFont) {
		surface->defaultFont = (DgUIFontMetrics *) DgAlloc(sizeof *surface->defaultFont);
	}
	
	*surface->defaultFont = *metrics;
}

void DgUISurfaceSetDefaultPanelMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics) {
	/**
	 * Set default panel metrics. This will make an interal copy of the structure.
	 */
	
	if (!surface->defaultPanel) {
		surface->defaultPanel = (DgUIPanelMetrics *) DgAlloc(sizeof *surface->defaultPanel);
	}
	
	*surface->defaultPanel = *metrics;
}

void DgUISurfaceSetDefaultSelectionMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics) {
	/**
	 * Set default text selection metrics. This will make an interal copy of the structure.
	 */
	
	if (!surface->defaultSelect) {
		surface->defaultSelect = (DgUIPanelMetrics *) DgAlloc(sizeof *surface->defaultSelect);
	}
	
	*surface->defaultSelect = *metrics;
}

/**
 * =============================================================================
 * CONSTRUCTORS
 * =============================================================================
 */

DgUIElementBase DgUIElementBaseNew(DgVec2 pos, DgVec2 size, bool visible, bool enable) {
	/**
	 * Create DgUIElementBase object
	 */
	
	DgUIElementBase obj;
	
	obj.pos = pos;
	obj.size = size;
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

DgUIPanelMetrics DgUIPanelMetricsNew(DgVec4 colour, DgVec4 padding, float radius) {
	/**
	 * Create DgUIPanelMetrics object
	 */
	
	DgUIPanelMetrics obj;
	
	obj.colour = colour;
	obj.padding = padding;
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

DgUIString DgUIStringNew(const char * const string) {
	/**
	 * Create DgUIString object
	 */
	
	DgUIString obj;
	
	obj.content = DgStrdup(string);
	obj.length = strlen(obj.content);
	
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
	obj.panel = (panel) ? panel : surface->defaultPanel;
	
	return obj;
}

DgUILabel DgUILabelNew(
	DgUISurface *surface, 
	DgUIElementBase *base,
	DgUIFontMetrics *font, 
	DgUIString *text) 
{
	/**
	 * Create a new label
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUILabel obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.font = (font) ? font : surface->defaultFont;
	obj.text = (text) ? *text : DgUIStringNew("Missing String");
	
	return obj;
}

DgUILineEdit DgUILineEditNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	DgUIString *placeholder, 
	DgUIString *text, 
	DgUIPanelMetrics *s_style)
{
	/**
	 * Create a new lineedit
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUILineEdit obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.font = (font) ? font : surface->defaultFont;
	obj.panel = (panel) ? panel : surface->defaultPanel;
	obj.placeholder = (placeholder) ? *placeholder : DgUIStringNew("Missing String");
	obj.text = (text) ? *text : DgUIStringNew("Missing String");
	obj.curpos = obj.text.length;
	
	obj.s_style = (s_style) ? s_style : surface->defaultPanel;
	
	return obj;
}

DgUIButton DgUIButtonNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	DgUIString *text,
	DgUIActionSpec *action)
{
	/**
	 * Create a new button
	 * 
	 * Surface can be NULL as long as all pointers are valid and not NULL.
	 */
	
	DgUIButton obj;
	
	obj.base = (base) ? *base : surface->defaultBase;
	obj.font = (font) ? font : surface->defaultFont;
	obj.panel = (panel) ? panel : surface->defaultPanel;
	obj.text = (text) ? *text : DgUIStringNew("Missing String");
	obj.action = (action) ? *action : (DgUIActionSpec) {.context = NULL, .callback = NULL};
	obj.pressed = false;
	
	return obj;
}

/**
 * =============================================================================
 * DESTRUCTORS
 * =============================================================================
 */

static void DgUIStringFree(DgUIString *obj) {
	/**
	 * Free a DgUIString object.
	 */
	
	DgFree((void *) obj->content);
}

static void DgUILineEditFree(DgUILineEdit *obj) {
	/**
	 * Free a DgUILineEdit object.
	 */
	
	DgUIStringFree(&obj->placeholder);
	DgUIStringFree(&obj->text);
}

static void DgUIButtonFree(DgUIButton *obj) {
	/**
	 * Free a DgUIButton object.
	 */
	
	DgUIStringFree(&obj->text);
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

void DgUISurfaceUpdate(DgUISurface *surface, DgUISurfaceUpdateStructure *info) {
	/**
	 * Call all of the needed update routines.
	 */
	
}

static uint8_t DgUISurfaceRenderPanel(DgUIRenderData *data, const DgUIPanel * const panel) {
	/**
	 * Generate a mesh for a panel.
	 */
	
	// Get vertex set
	DgUIRenderVertexSet *vs = &data->data[data->length++];
	
	memset(vs, 0, sizeof *vs);
	
	vs->vertex = (DgUIRenderVertex *) DgAlloc(sizeof *vs->vertex * 4);
	
	if (!vs->vertex) {
		return 1;
	}
	
	vs->index = (uint32_t *) DgAlloc(sizeof *vs->index * 6);
	
	if (!vs->index) {
		return 2;
	}
	
	vs->vertex[0] = (DgUIRenderVertex) { 
		panel->base.pos.x, panel->base.pos.y, 
		0.0f, 0.0f, 
		panel->panel->colour.r, panel->panel->colour.g, 
		panel->panel->colour.b, panel->panel->colour.a };
	
	vs->vertex[1] = (DgUIRenderVertex) { 
		panel->base.pos.x, panel->base.pos.y - panel->base.size.y, 
		0.0f, 0.0f, 
		panel->panel->colour.r, panel->panel->colour.g, 
		panel->panel->colour.b, panel->panel->colour.a };
	
	vs->vertex[2] = (DgUIRenderVertex) { 
		panel->base.pos.x + panel->base.size.x, panel->base.pos.y - panel->base.size.y, 
		0.0f, 0.0f, 
		panel->panel->colour.r, panel->panel->colour.g, 
		panel->panel->colour.b, panel->panel->colour.a };
	
	vs->vertex[3] = (DgUIRenderVertex) { 
		panel->base.pos.x + panel->base.size.x, panel->base.pos.y, 
		0.0f, 0.0f, 
		panel->panel->colour.r, panel->panel->colour.g, 
		panel->panel->colour.b, panel->panel->colour.a };
	
	vs->index[0] = 0;
	vs->index[1] = 1;
	vs->index[2] = 2;
	vs->index[3] = 0;
	vs->index[4] = 2;
	vs->index[5] = 3;
	
	return 0;
}

static uint8_t DgUISrufaceRenderAlloc(DgUIRenderData *data) {
	/**
	 * Make sure there are enough VertexSet structures allocated. If not, 
	 * allocate more.
	 */
	
	if (data->cache <= data->length) {
		data->cache = 4 + data->cache * 2;
		data->data = (DgUIRenderVertexSet *) DgRealloc(data->data, sizeof *data->data * data->cache);
		
		if (!data->data) {
			return 1;
		}
	}
	
	return 0;
}

void DgUISurfaceRenderData(DgUISurface *surface, DgUIRenderData *data) {
	/**
	 * Generate and return the render data.
	 */
	
	memset(data, 0, sizeof *data);
	
	for (size_t i = 0; i < surface->panel_count; i++) {
		if (DgUISrufaceRenderAlloc(data)) return;
		if (DgUISurfaceRenderPanel(data, &surface->panel[i])) return;
	}
}

void DgUISurfaceRenderDataFree(DgUIRenderData *data) {
	/**
	 * Free all of the render data for a given panel.
	 */
	
	for (size_t i = 0; i < data->length; i++) {
		DgUIRenderVertexSet *set = &data->data[i];
		
		DgFree(set->vertex);
		DgFree(set->index);
	}
	
	DgFree(data->data);
}
