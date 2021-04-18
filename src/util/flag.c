/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * One of a few ways to implement observers. Should be rewritten.
 */

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"

#include "flag.h"

typedef struct {
	DgFlagEventInfo* events;
	size_t allocated;
	size_t left; // The number of remaing free spots 
	             // (remember that flags can't be deallocated)
} DgFlagSystem;

DgFlagSystem dg_flag;

bool DgFlagCreateEvent(const char* name) {
	/**
	 * <summary>
	 * DEPRECATED: This API and system will be replaced with the Events API soon.
	 * 
	 * Should be called to create an event with a name. Returns the index of
	 * the flag.
	 * </summary>
	 */
	printf("libmelon: \033[1;33mWarning:\033[0m Deprecated function DgFlagCreateEvent(name = \"%s\") was called.\n", name);
	
	// Initialise subsystem for the first time
	if (!dg_flag.events) {
		const size_t init_size = 16;
		
		dg_flag.events = (DgFlagEventInfo *) DgAlloc(sizeof(DgFlagEventInfo) * init_size);
		
		if (!dg_flag.events) {
			printf("Error: Failed to create event because allocation failed.\n");
			return false;
		}
		
		dg_flag.allocated = init_size;
		dg_flag.left = dg_flag.allocated;
	}
	
	// Resize if not large enough
	if (!dg_flag.left) {
		const size_t add_new = 4;
		
		dg_flag.events = (DgFlagEventInfo *) DgRealloc(dg_flag.events, dg_flag.allocated + sizeof(DgFlagEventInfo) * add_new);
		
		if (!dg_flag.events) {
			printf("Error: Failed to create event because reallocation failed.\n");
			return false;
		}
		
		dg_flag.allocated += add_new;
		dg_flag.left += add_new;
	}
	
	// Register the event and name
	DgFlagEventInfo* this = dg_flag.events + (dg_flag.allocated - dg_flag.left);
	
	this->name = (char *) DgAlloc((strlen(name) + 1) * sizeof(char));
	
	if (!this->name) {
		printf("Error: Failed to allocate memory for string name.");
		return false;
	}
	
	strcpy(this->name, name);
	this->callbacks = NULL;
	this->callback_count = 0;
	
	dg_flag.left--;
	
	return true;
}

bool DgFlagRegisterCallback(const char* event, void (*func)(const char*, void*)) {
	/* Registers a callback to an event */
	printf("libmelon: \033[1;33mWarning:\033[0m Deprecated function DgFlagRegisterCallback(event = \"%s\", func = <%X>) was called.\n", event, func);
	
	DgFlagEventInfo* this = NULL;
	
	// Find the event we want to work with
	for (int i = 0; i < (dg_flag.allocated - dg_flag.left); i++) {
		if (!strcmp(dg_flag.events[i].name, event)) {
			this = dg_flag.events + i;
			break;
		}
	}
	
	if (!this) {
		printf("__FILE__:__LINE__ - No such event '%s'.\n", event);
		return false;
	}
	
	// Create function pointer array if it doesn't exsist.
	if (!this->callbacks) {
		this->callbacks = (void(**)(const char*,void*)) DgAlloc(sizeof(void(*)(const char*,void*)));
		
		if (!this->callbacks) {
			printf("__FILE__:__LINE__ - Failed to allocate memory.\n");
			return false;
		}
	}
	else {
		this->callbacks = (void(**)(const char*,void*)) DgRealloc(this->callbacks, sizeof(void(*)(const char*,void*)) * (this->callback_count + 1));
		
		if (!this->callbacks) {
			printf("__FILE__:__LINE__ - Failed to allocate memory.\n");
			return false;
		}
	}
	
	// Assign function pointer
	this->callbacks[this->callback_count] = func;
	this->callback_count++;
	
	return true;
}

void DgFlagRaise(const char* event, void* params) {
	/* Triggers an event (raises its flag) */
	DgFlagEventInfo* this = NULL;
	
	// Find the event we want to work with
	for (int i = 0; i < (dg_flag.allocated - dg_flag.left); i++) {
		if (!strcmp(dg_flag.events[i].name, event)) {
			this = dg_flag.events + i;
			break;
		}
	}
	
	if (!this) {
		printf("__FILE__:__LINE__ - No such event '%s'.\n", event);
		return;
	}
	
	// Actually run the events
	// TODO: Make these seprate threads or fibres
	for (int i = 0; i < this->callback_count; i++) {
		(this->callbacks[i])(event, params);
	}
}

void DgFlagUpdate() {
	printf("Warning: DgFlagUpdate() does not work yet!\n");
}

void DgFlagGlobalCleanup() {
	if (dg_flag.events) {
		DgFree(dg_flag.events);
	}
}
