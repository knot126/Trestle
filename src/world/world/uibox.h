/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * UI Box Component functions
 */

bool ui_element_set_box(World * const restrict world, const uint32_t id, const DgVec2 size, const DgVec2 pos) {
	C_UIBox *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->box[i].base.id == id) {
			element = &world->ui->box[i];
			break;
		}
	}
	
	if (!element) {
		return false;
	}
	
	element->pos = pos;
	element->size = size;
	element->updated = true;
	
	return true;
}

bool ui_element_set_box_colour(World * const restrict world, const uint32_t id, const DgVec4 colour) {
	C_UIBox *element = NULL;
	
	if (!world->ui) {
		return false;
	}
	
	for (uint32_t i = 0; i < world->ui->text_count; i++) {
		if (world->ui->box[i].base.id == id) {
			element = &world->ui->box[i];
			break;
		}
	}
	
	if (!element) {
		return false;
	}
	
	element->colour = colour;
	element->updated = true;
	
	return true;
}
