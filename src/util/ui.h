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

// Any element
typedef struct DgUIElementBase {
	DgVec2 pos;                        // Position
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
	DgVec2 size;                       // Size of the panel
	float radius;                      // Curve radius
} DgUIPanelMetrics;

typedef struct DgUIActionSpec {
	void *context;                     // Context to be passed to callback
	void (*callback)(void *context);   // Callback to be called on action
};

// Actual UI elements

typedef struct DgUIPanel {
	DgUIElementBase base;
	DgUIPanelMetrics panel;
	// =========================================================================
};

typedef struct DgUILabel {
	DgUIElementBase base;
	DgUIFontMetrics font;
	// =========================================================================
	const char *content;               // The string, may/may not be null terminated
	size_t length;                     // Length of the string
} DgUILabel;

typedef struct DgUILineEdit {
	DgUIElementBase base;
	DgUIFontMetrics font;
	DgUIPanelMetrics panel;
	// =========================================================================
	const char *placeholder;           // Text that appears when no content is there
	size_t placeholder_length;         // The placeholder text length
	
	const char *content;               // Content that the user has input
	size_t length;                     // The length of the user content
	size_t curpos;                     // The current position of the cursor
	
	DgUIPanelMetrics s_style;          // Info about the selection type
	size_t s_start, s_end;             // End and start of selection
	bool selecting;                    // If the user is selecting things
} DgUILineEdit;

typedef struct DgUIButton {
	DgUIElementBase base;
	DgUIFontMetrics font;
	DgUIPanelMetrics panel;
	// =========================================================================
	const char *content;               // Text that appears on the button
	size_t length;                     // The placeholder text length
	DgUIActionSpec action;             // Action to preform when button is pressed
	bool pressed;                      // If this button was recently pressed
} DgUIButton;

// Surfaces

#define DG_UI_ELEMENT_LIST(TYPE_NAME, ACCESS_NAME, COUNT_NAME) TYPE_NAME ACCESS_NAME; size_t COUNT_NAME; 

typedef struct DgUISurface {
	// The default for some styling elements
	DgUIElementBase defaultBase;
	DgUIFontMetrics defaultFont;
	DgUIPanelMetrics defaultPanel;
	
	// The lists of elements
	DG_UI_ELEMENT_LIST(DgUIPanel, panel, panel_count);
	DG_UI_ELEMENT_LIST(DgUILabel, label, label_count);
	DG_UI_ELEMENT_LIST(DgUILineEdit, lineedit, lineedit_count);
	DG_UI_ELEMENT_LIST(DgUIButton, button, button_count);
} DgUISurface;

#undef DG_UI_ELEMENT_LIST

// Function Declarations

DgUISurface *DgUISurfaceNew(void);
void DgUISurfaceFree(DgUISurface *surface);

void DgUISurfaceSetDefaultBase(DgUISurface *surface, DgUIElementBase *base);
void DgUISurfaceSetDefaultFontMetrics(DgUISurface *surface, DgUIFontMetrics *metrics);
void DgUISurfaceSetDefaultPanelMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics);
