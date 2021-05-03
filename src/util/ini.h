/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games INI Parser v1
 */
 
#pragma once

#include <stdbool.h>
#include <inttypes.h>

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
