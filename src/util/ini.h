/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games INI Parser v1
 */
 
#pragma once

#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct {
	char *key;
	char *value;
} DgINIPair;

typedef struct {
	char *title;
	DgINIPair *pairs;
	size_t pair_count;
} DgINISection;

typedef struct {
	DgINISection *sections;
	size_t section_count;
} DgINIDocument;

uint32_t DgINIParse(DgINIDocument *doc, const uint32_t length, const char * const content);
uint32_t DgINILoad(DgINIDocument *doc, char * const path);
void DgINIFree(DgINIDocument *doc);

char *DgINIGet(const DgINIDocument * const doc, const char * const sect, const char * const key, char *safe);
void DgINIPrint(DgINIDocument *doc);