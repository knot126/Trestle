/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GUI and Widget Library
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "maths.h"

enum {
	DG_UI_INVALID = 0,
	
	DG_UI_FRAME = 1,
	DG_UI_LABEL = 2,
	DG_UI_LINEEDIT = 3,
	DG_UI_BUTTON = 4,
};

enum {
	DG_UI_LEFT = 1,
	DG_UI_CENTRE = 2,
	DG_UI_RIGHT = 3,
};

/**
 * =============================================================================
 * Base Types - used in elements and for communication with client
 * =============================================================================
 */

typedef struct DgUIElementBase {
	DgVec2 pos;                        // Position
	DgVec2 size;                       // Size for panels and buttons
	bool visible;                      // Weather or not the element is visible
	bool enable;                       // Weather or not the element is enabled
} DgUIElementBase;

typedef struct DgUIFontMetrics {
	DgVec4 colour;                     // Colour
	float size;                        // Size of the text
	int32_t font;                      // ID of the font
	uint32_t align;                    // Alignment of the text
} DgUIFontMetrics;

typedef struct DgUIPanelMetrics {
	DgVec4 colour;                     // Colour of the panel
	DgVec4 padding;                    // Padding on all sides of the panel
	float radius;                      // Curve radius
} DgUIPanelMetrics;

typedef struct DgUIActionSpec {
	void *context;                     // Context to be passed to callback
	void (*callback)(void *context);   // Callback to be called on action
} DgUIActionSpec;

typedef struct DgUIString {
	const char *content;               // The string, may/may not be null terminated
	size_t length;                     // Length of the string
} DgUIString;

/**
 * =============================================================================
 * UI Elements - the types of things that can appear in a UI
 * =============================================================================
 */

typedef struct DgUIPanel {
	DgUIElementBase base;
	DgUIPanelMetrics *panel;
	// =========================================================================
} DgUIPanel;

typedef struct DgUILabel {
	DgUIElementBase base;
	DgUIFontMetrics *font;
	// =========================================================================
	DgUIString text;                   // The text to be displayed
	DgUIActionSpec action;             // Action to preform when label is pressed
	bool pressed;                      // If this label was recently pressed
} DgUILabel;

typedef struct DgUILineEdit {
	DgUIElementBase base;
	DgUIFontMetrics *font;
	DgUIPanelMetrics *panel;
	// =========================================================================
	DgUIString placeholder;            // The placeholder text
	DgUIString text;                   // The text that the user has entered
	size_t curpos;                     // The current position of the cursor
	
	DgUIPanelMetrics *s_style;         // Info about the selection type
	size_t s_start, s_end;             // End and start of selection
	bool selecting;                    // If the user is selecting things
} DgUILineEdit;

typedef struct DgUIButton {
	DgUIElementBase base;
	DgUIFontMetrics *font;
	DgUIPanelMetrics *panel;
	// =========================================================================
	DgUIString text;                   // The text that appears on the button
	DgUIActionSpec action;             // Action to preform when button is pressed
	bool pressed;                      // If this button was recently pressed
} DgUIButton;

/**
 * =============================================================================
 * Surfaces - the things that UI elements are part of, represents the whole UI
 * =============================================================================
 */

typedef struct DgUIActiveInfo {
	size_t type;
	size_t index;
} DgUIActiveInfo;

#define DG_UI_ELEMENT_LIST(TYPE_NAME, ACCESS_NAME, COUNT_NAME) TYPE_NAME *ACCESS_NAME; size_t COUNT_NAME; 

typedef struct DgUISurface {
	// The default for some styling elements
	DgUIElementBase defaultBase;
	DgUIFontMetrics *defaultFont;
	DgUIPanelMetrics *defaultPanel;
	DgUIPanelMetrics *defaultSelect;
	
	DgUIActiveInfo active;
	
	// The lists of elements
	DG_UI_ELEMENT_LIST(DgUIPanel, panel, panel_count);
	DG_UI_ELEMENT_LIST(DgUILabel, label, label_count);
	DG_UI_ELEMENT_LIST(DgUILineEdit, lineedit, lineedit_count);
	DG_UI_ELEMENT_LIST(DgUIButton, button, button_count);
} DgUISurface;

#undef DG_UI_ELEMENT_LIST

/**
 * =============================================================================
 * Update Structures - information from the client about how to update the
 * UI state
 * =============================================================================
 */

typedef struct DgUISurfaceUpdateStructure {
	unsigned(*getKey)(void *ctx, unsigned code);
	void *getKey_ctx;
} DgUISurfaceUpdateStructure;

/**
 * =============================================================================
 * Vertex Data Structures - data containing info on the vertex data
 * =============================================================================
 */

typedef struct DgUIRenderVertex {
	// A single vertex for the UI
	float x, y;
	float u, v;
	float r, g, b, a;
} DgUIRenderVertex;

typedef struct DgUIRenderVertexSet {
	// A set of vertexes
	DgUIRenderVertex *vertex;
	size_t vertex_count;
	
	uint32_t *index;
	uint32_t index_count;
	
	uint32_t flags;
	uint32_t texture;
} DgUIRenderVertexSet;

typedef struct DgUIRenderData {
	/* Data needed to render the UI. */
	DgUIRenderVertexSet *data;
	size_t length;
} DgUIRenderData;

/**
 * =============================================================================
 * Functions - the exposed API
 * =============================================================================
 */

DgUISurface *DgUISurfaceNew(void);
void DgUISurfaceFree(DgUISurface *surface);

void DgUISurfaceSetDefaultBase(DgUISurface *surface, DgUIElementBase *base);
void DgUISurfaceSetDefaultFontMetrics(DgUISurface *surface, DgUIFontMetrics *metrics);
void DgUISurfaceSetDefaultPanelMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics);
void DgUISurfaceSetDefaultSelectionMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics);

DgUIElementBase DgUIElementBaseNew(DgVec2 pos, DgVec2 size, bool visible, bool enable);
DgUIFontMetrics DgUIFontMetricsNew(DgVec4 colour, float size, int32_t font, uint32_t align);
DgUIPanelMetrics DgUIPanelMetricsNew(DgVec4 colour, DgVec4 padding, float radius);
DgUIActionSpec DgUIActionSpecNew(void *context, void (*callback)(void *context));

DgUIPanel DgUIPanelNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIPanelMetrics *panel);

DgUILabel DgUILabelNew(
	DgUISurface *surface, 
	DgUIElementBase *base,
	DgUIFontMetrics *font, 
	DgUIString *text);

DgUILineEdit DgUILineEditNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	DgUIString *placeholder, 
	DgUIString *text, 
	DgUIPanelMetrics *s_style);

DgUIButton DgUIButtonNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	DgUIString *text,
	DgUIActionSpec *action);
