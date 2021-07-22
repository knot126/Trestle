/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GUI and Widget Library
 */

#pragma once

/*
================================================================================

# Using the Melon GUI system

## Surfaces

The Decent Games GUI library has the concept of surfaces on which your UI 
elements exist. The surface contains all of the state, since the UI library does
not introduce any global state into the application. You can create and destroy 
a surface using:

```c
DgUISurface *DgUISurfaceNew(void);
void DgUISurfaceFree(DgUISurface *surface);
```

## Elements

This library also has the concept of elements: elements are basically "objects"
that represent one UI thing on the screen. For example, a button or a line edit.
They can be created using one of these functions:

```c
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
```

These functions will return a new object by value, which can then be passed on 
to one of the functions that will add or remove them from the surface:

```c
size_t DgUISurfaceAddPanel(DgUISurface *surface, DgUIPanel obj);
size_t DgUISurfaceAddLabel(DgUISurface *surface, DgUILabel obj);
size_t DgUISurfaceAddLineEdit(DgUISurface *surface, DgUILineEdit obj);
size_t DgUISurfaceAddButton(DgUISurface *surface, DgUIButton obj);

size_t DgUISurfaceRemovePanel(DgUISurface *surface, size_t ident);
size_t DgUISurfaceRemoveLabel(DgUISurface *surface, size_t ident);
size_t DgUISurfaceRemoveLineEdit(DgUISurface *surface, size_t ident);
size_t DgUISurfaceRemoveButton(DgUISurface *surface, size_t ident);
```

## The Update Loop

The update loop is designed to be as simple as possible while involing very 
little global state and caching. Once for every update frame, make sure to call 
`DgUISurfaceUpdate(DgUISurface *surface, DgUISurfaceUpdateStructure *info)` so
that the selection and active elements, as well as keyboard keypresses, are
updated. Do note that this won't render the UI elements; for that, you will need
to get the UI vertex data using these functions:

```c 
void DgUISurfaceRenderData(DgUISurface *surface, DgUIRenderData *data);
void DgUISurfaceRenderDataFree(DgUIRenderData *data);
```

This will produce some vertex data that you can render yourself. It is stored in
sets of vertexes, which should be drawn in the order they are in the list.

================================================================================
*/

#include <inttypes.h>
#include <stdbool.h>

#include "maths.h"

// UI element types
enum {
	DG_UI_INVALID = 0,
	
	DG_UI_FRAME = 1,
	DG_UI_LABEL = 2,
	DG_UI_LINEEDIT = 3,
	DG_UI_BUTTON = 4,
};

// UI alignment types
enum {
	DG_UI_LEFT = 1,
	DG_UI_RIGHT = 2,
	DG_UI_CENTRE = 3,
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
 * Functions for adding and removing elements from a surface
 * =============================================================================
 */

size_t DgUISurfaceAddPanel(DgUISurface *surface, DgUIPanel obj);
size_t DgUISurfaceAddLabel(DgUISurface *surface, DgUILabel obj);
size_t DgUISurfaceAddLineEdit(DgUISurface *surface, DgUILineEdit obj);
size_t DgUISurfaceAddButton(DgUISurface *surface, DgUIButton obj);

size_t DgUISurfaceRemovePanel(DgUISurface *surface, size_t ident);
size_t DgUISurfaceRemoveLabel(DgUISurface *surface, size_t ident);
size_t DgUISurfaceRemoveLineEdit(DgUISurface *surface, size_t ident);
size_t DgUISurfaceRemoveButton(DgUISurface *surface, size_t ident);

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
	size_t cache;
} DgUIRenderData;

/**
 * =============================================================================
 * Functions - the exposed API
 * =============================================================================
 */

// Surface
DgUISurface *DgUISurfaceNew(void);
void DgUISurfaceFree(DgUISurface *surface);

// Bases - set default
void DgUISurfaceSetDefaultBase(DgUISurface *surface, DgUIElementBase *base);
void DgUISurfaceSetDefaultFontMetrics(DgUISurface *surface, DgUIFontMetrics *metrics);
void DgUISurfaceSetDefaultPanelMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics);
void DgUISurfaceSetDefaultSelectionMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics);

// Bases - construct
DgUIElementBase DgUIElementBaseNew(DgVec2 pos, DgVec2 size, bool visible, bool enable);
DgUIFontMetrics DgUIFontMetricsNew(DgVec4 colour, float size, int32_t font, uint32_t align);
DgUIPanelMetrics DgUIPanelMetricsNew(DgVec4 colour, DgVec4 padding, float radius);
DgUIActionSpec DgUIActionSpecNew(void *context, void (*callback)(void *context));

// Elements - construct
// 
// These functions will help creating a new type of element that can be passed
// to DgUISurfaceAdd<type>.
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

// Drawing and Update API
void DgUISurfaceUpdate(DgUISurface *surface, DgUISurfaceUpdateStructure *info);
void DgUISurfaceRenderData(DgUISurface *surface, DgUIRenderData *data);
void DgUISurfaceRenderDataFree(DgUIRenderData *data);
