/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Audio Related Abstractions
 */ 

#include <inttypes.h>

#include "audio.h"

#if defined(__linux__)
#include <pulse/simple.h>

uint32_t DgAudioStreamNew(DgAudioStream *stream) {
	/**
	 * Initialise an audio stream.
	 */
	
	stream->spec.format = PA_SAMPLE_FLOAT32LE;
	stream->spec.channels = 1;
	stream->spec.rate = 44100;
	
	stream->backend = pa_simple_new(NULL, "Melon Software Framework", PA_STREAM_PLAYBACK, NULL, "Melon Software Framework", &stream->spec, NULL, NULL, NULL);
}

void DgAudioStreamPush(DgAudioStream *stream, size_t size, void *data) {
	/**
	 * Try to write data to the audio server.
	 */
	
	pa_simple_write(stream->backend, data, size, NULL);
}

void DgAudioStreamFree(DgAudioStream *stream) {
	pa_simple_free(stream->backend);
}
#endif
