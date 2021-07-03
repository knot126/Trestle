# Melon UI

This document documents the Melon UI library. It can be included by including `util/ui.h`.

## Design Quick Notes

  * All strings are stored using their length and **ARE NOT** null terminated.
  * While structures are provided as a nice reference in this document, you should instead use the constructor functions when making any specific structure. Constructor functions are always named after the type name followed by the word "New".
  * There is no element heirarchy as this greatly simplifies the API. If a more complex system is needed, use a different UI library.
  * This library also does not automatically lay out elements for you. You must do that yourself.

## Surface

The surface is what is used to contain all of the UI elements. You can create a surface using `surface = DgUISurfaceNew()`, which returns a new surface. To destory a surface, use `DgUISurfaceFree(surface)`.

Here is what a surface looks like internally:

```c
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
```

## Elements

Elements make up the core of the User Interface. They can display information, provide means to trigger an action and let the user provide input. Elements are stored in Surfaces.

### Overview of Types

Here are the types of elements:

  * **Label**: Display strings of text.
  * **Panel**: Display background boxes, for visual organisation.
  * **LineEdit**: Let the user edit a single line of text.
  * **Button**: Allow the user to trigger actions.
  * *More should be added...*

### Common Structures

#### Base Struct

All elements include a base struct, which just has the position information as well as two booleans to store if the element is visible and enabled.

```c
typedef struct DgUIElementBase {
	DgVec2 pos;                        // Position
	bool visible;                      // Weather or not the element is visible
	bool enable;                       // Weather or not the element is enabled
} DgUIElementBase;
```

#### Font Metrics

There is also a font metrics style info struct, which is commonly used for styling text elements. It provides colour (RGBA), the size (in OpenGL-like standard units) in height and how the text is aligned (left, centre or right).

```c
typedef struct DgUIFontMetrics {
	DgVec4 colour;                     // Colour
	float size;                        // Size of the text
	int32_t font;                      // ID of the font
	uint32_t align;                    // Alignment of the text
} DgUIFontMetrics;
```

#### Panel Metrics

In addition, there is a panel metrics structure which takes care of boxes (panels). For example, the background of a LineEdit, a Panel and a Button all use a box in the background to differintiate the regions where they exist and can be interacted with. That box has its information provided by this structure.

This structure provides the colour of the panel (RGBA), how much padding it should have on each side, the size of the panel in standard units and the radius of the rounded conrners (not required to be supported by an implmentation).

In the future, this may be updated to allow for providing stroke (outline) information.

```c
typedef struct DgUIPanelMetrics {
	DgVec4 colour;                     // Colour of the panel
	DgVec4 padding;                    // Padding on all sides of the panel
	DgVec2 size;                       // Size of the panel
	float radius;                      // Curve radius
} DgUIPanelMetrics;
```

#### Action (callback)

There is a structure that can contain a function pointer that accepts a callback that takes a single `void *` and stores a `void *` that will be passed to it. This is used for events and actions, like pressing a button.

```c
typedef struct DgUIActionSpec {
	void *context;                     // Context to be passed to callback
	void (*callback)(void *context);   // Callback to be called on action
};
```

### Types of Elements

#### Panel

A panel works like you might expect a panel to work in any other collection of widgets: it simpily provides a surface for rendering things on top of. Note that since Melon UI does not have a heiarcy, this is only for organisation as things are seen, not for the implementation.

```c
typedef struct DgUIPanel {
	DgUIElementBase base;
	DgUIPanelMetrics panel;
	// =========================================================================
};
```

#### Label

A label provides a means to output text without outputting anything else.

```c
typedef struct DgUILabel {
	DgUIElementBase base;
	DgUIFontMetrics font;
	// =========================================================================
	const char *content;               // The string, may/may not be null terminated
	size_t length;                     // Length of the string
} DgUILabel;
```

#### LineEdit

A LineEdit provides a means to edit a single line of text - that is, text without `\n` or `\r`. This line editor element should have all the basic things to expect from a line editing element, without having anything else.

This should support highlighting text as well as inserting and deleting text at any point in the string, as well as copy and pasting.

```c
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
```

#### Button

A Button usually provides a means to preform an action. The Button can either be checked between UI updates for weather it was pressed, or it can optionally use a callback.

```c
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
```

## Full API

This is probably the useful part of this document for just creating interfaces. It describes the full API and how it should be used.

### Surfaces

#### Set Defaults

You can set the default base struct, font and panel metrics using these functions:

```c
void DgUISurfaceSetDefaultBase(DgUISurface *surface, DgUIElementBase *base);
void DgUISurfaceSetDefaultFontMetrics(DgUISurface *surface, DgUIFontMetrics *metrics);
void DgUISurfaceSetDefaultPanelMetrics(DgUISurface *surface, DgUIPanelMetrics *metrics);
```

#### Create Simple Objects

Simpler objects can be created using the "New" series of functions:

```c
DgUIElementBase DgUIElementBaseNew(DgVec2 pos, bool visible, bool enable);
DgUIFontMetrics DgUIFontMetricsNew(DgVec4 colour, float size, int32_t font, uint32_t align);
DgUIPanelMetrics DgUIPanelMetricsNew(DgVec4 colour, DgVec4 padding, DgVec2 size, float radius);
DgUIActionSpec DgUIActionSpecNew(void *context, void (*callback)(void *context));
```

Examples:

```c
DgUIPanelMetrics pm = DgUIPanelMetricsNew(
	DgVec4New(0.0f, 0.0f, 0.0f, 1.0f),
	DgVec4New(0.05f, 0.05f, 0.05f, 0.05f),
	DgVec2New(0.5f, 0.3f),
	0.035f
);
```

#### Create Elements (Complex Objects)

More complex objects - like Elements - are created similarly but may require that memory be allocated or allow you to pass in a surface to get defaults.

```c
DgUIPanel DgUIPanelNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIPanelMetrics *panel);

DgUILabel DgUILabelNew(
	DgUISurface *surface, 
	DgUIElementBase *base,
	DgUIFontMetrics *font, 
	const char * const content);

DgUILineEdit DgUILineEditNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	const char * const placeholder, 
	const char * const content, 
	DgUIPanelMetrics *s_style);

DgUIButton DgUIButtonNew(
	DgUISurface *surface, 
	DgUIElementBase *base, 
	DgUIFontMetrics *font, 
	DgUIPanelMetrics *panel, 
	const char * const content,
	DgUIActionSpec *action);
```

Example: 

```c
DgUILineEdit le = DgUILineEditNew(
	NULL, 
	&DgUIElementBaseNew(
		DgVec2New(-1.0f, 1.0f), // top corner
		true,
		true
	), 
	&DgUIFontMetricsNew(
		DgVec4New(1.0f, 1.0f, 1.0f, 0.975f),
		0.05f,
		0,
		DG_UI_LEFT
	), 
	&DgUIPanelMetricsNew(
		DgVec4New(0.0f, 0.0f, 0.0f, 0.5f),
		DgVec4New(0.03f, 0.03f, 0.03f, 0.03f),
		DgVec2New(0.2f, 0.1f),
		0.035f
	), 
	"Type here...", 
	"", 
	&DgUIPanelMetricsNew(
		DgVec4New(1.0f, 1.0f, 1.0f, 0.1f),
		DgVec4New(0.0f, 0.0f, 0.0f, 0.0f),
		DgVec2New(0.0f, 0.0f), // not used
		0.035f
	)
);

DgUISurfaceAddLineEdit(surface, &le);
```
