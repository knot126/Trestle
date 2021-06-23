/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Audio Related Abstractions
 */

#pragma once

#include <inttypes.h>

#if defined(__linux__)
#include <pulse/simple.h>

typedef struct DgAudioStream {
	pa_simple *backend;
	pa_sample_spec spec;
} DgAudioStream;
#endif

uint32_t DgAudioStreamNew(DgAudioStream *stream);
void DgAudioStreamPush(DgAudioStream *stream, size_t size, void *data);
void DgAudioStreamFree(DgAudioStream *stream);
