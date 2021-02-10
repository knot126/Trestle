/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * One of a few ways to implement observers. Should be rewritten.
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct DgFlagEventInfo {
	char* name;                     // Name of the event
	void (**callbacks)(const char*, void*); // Array of function pointers to callbacks
	void* params;                   // Paramaters for the event
	size_t callback_count;          // Number of callbacks this event has
} DgFlagEventInfo;

bool DgFlagCreateEvent(const char* name);
bool DgFlagRegisterCallback(const char* event, void (*func)(const char*, void*));
void DgFlagRaise(const char* event, void* params);
void DgFlagUpdate();
void DgFlagGlobalCleanup();
