/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Old UI header
 */

typedef enum {
	QR_ELEMUI_BOX = (1 << 0),
	QR_ELEMUI_TEXT = (1 << 1),
} UIElementMaskEnum;

// UI Text
bool ui_element_set_text(World * const restrict world, const uint32_t id, const char * const restrict text);
bool ui_element_set_text_pos(World * const restrict world, const uint32_t id, const DgVec2 pos);
bool ui_element_set_text_size(World * const restrict world, const uint32_t id, const float size);
bool ui_element_set_text_font(World * const restrict world, const uint32_t id, const char * const restrict font);
bool ui_element_set_text_colour(World * const restrict world, const uint32_t id, const DgVec4 colour);

// UI Box 
bool ui_element_set_box(World * const restrict world, const uint32_t id, const DgVec2 size, const DgVec2 pos);
bool ui_element_set_box_colour(World * const restrict world, const uint32_t id, const DgVec4 colour);
