/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * UI Text Component functions
 */

/**
 * -----------------------------------------------------------------------------
 * UI Text
 * -----------------------------------------------------------------------------
 */

bool ui_element_set_text(World * const restrict world, const uint32_t id, const char * const restrict text) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		return false;
	}
	
	if (element->text) {
		DgFree((char *) element->text);
	}
	
	element->text = DgStrdup((char *) text);
	
	if (!element->text) {
		return false;
	}
	
	element->updated = true;
	
	return true;
}

bool ui_element_set_text_pos(World * const restrict world, const uint32_t id, const DgVec2 pos) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		DgLog(DG_LOG_ERROR, "Failed to find text for UI element: %d", id);
		return false;
	}
	
	element->pos = pos;
	
	element->updated = true;
	
	return true;
}

bool ui_element_set_text_size(World * const restrict world, const uint32_t id, const float size) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		DgLog(DG_LOG_ERROR, "Failed to find text for UI element: %d", id);
		return false;
	}
	
	element->size = size;
	
	element->updated = true;
	
	return true;
}

bool ui_element_set_text_font(World * const restrict world, const uint32_t id, const char * const restrict font) {
	C_UIText *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->text[i].base.id == id) {
			element = &world->ui->text[i];
			break;
		}
	}
	
	if (!element) {
		DgLog(DG_LOG_ERROR, "Failed to find text for UI element: %d", id);
		return false;
	}
	
	if (element->font) {
		DgFree(element->font);
	}
	
	element->font = DgStrdup(font);
	
	// NOTE:
	// We don't need to set the updated feild for fonts, since that can be applied
	// without changing vertex data...
	
	return true;
}
