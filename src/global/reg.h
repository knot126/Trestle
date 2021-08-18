/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Registry for global settings
 */ 

#pragma once

typedef enum RegType {
	QR_REG_NUMBER,
	QR_REG_INTEGER,
	QR_REG_STRING,
} RegType;

typedef struct Registry {
	const char **key;
	uint32_t    *hash;
	char       **value;
	size_t count;
	size_t alloc;
} Registry;

void registry_init(Registry * const restrict t);
void registry_free(Registry * const restrict t);

int8_t registry_set(Registry * const restrict t, const char * k, char * v);
char *registry_get(Registry * const restrict t, const char * k);
