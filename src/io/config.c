/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * SimpleConfig File Lodader
 */

#include <string.h>
#include <stdbool.h>

#include "../util/bag.h"
#include "../util/alloc.h"
#include "../util/fail.h"
#include "../io/fs.h"
#include "../io/load.h"

#include "config.h"

const bool _DG_SIMPLE_CONFIG_PARSER_DEBUG = false;
//#define _DG_SIMPLE_CONFIG_PARSER_CLOCK 1

#if defined(_DG_SIMPLE_CONFIG_PARSER_CLOCK)
	#include "../util/time.h"
#endif

DgConfig *DgConfigLoad(char *path, const bool enable_comments) {
	/* This will load a very simple configuation file into memory and decipher 
	 * it into a DgBag, which is then wrapped in a DgConfig structure. While
	 * it could have returned a property bag, the user would have to call a 
	 * special but unrelated function to free the allocated memory. There are
	 * some parts of this that are messy, but I plan to work on that with the 
	 * INI parser. */
	path = DgEvalPath(path);
	
	DgLoadBinaryFileInfo *file = DgLoadBinaryFile(path);
	
	// Free path name string
	DgFree(path);
	
	// Initialise the property bag
	DgConfig *conf = DgAlloc(sizeof(DgConfig));
	if (!conf) { return (void *) 0; } // failed to allocate memory
	conf->config = DgBagInit();
	
	if (_DG_SIMPLE_CONFIG_PARSER_DEBUG) {
		printf("SimpleConfig: Start parse file.\n");
	}
	
#if defined(_DG_SIMPLE_CONFIG_PARSER_CLOCK)
	float __ck = DgTime();
#endif
	
	// NOTE: Here, breadth is the current offset in the string.
	for (size_t breadth = 0; breadth < file->size; /* will inc in function */) {
		char buffer[256] = { 0 };
		
		if (_DG_SIMPLE_CONFIG_PARSER_DEBUG) {
			printf("SimpleConfig: Parsing line at b=%d.\n", breadth);
		}
		
		// Find the next new line, where base is the start of the current line
		char *base = (char *) ( (void*) file->data + breadth);
		char *end = (char *) memchr(base, '\n', (breadth - file->size));
		if (end == 0) { break; } // not safe to continue
		
		// Calculates size to copy using base (starting) and end (ending) pointers
		size_t line_len = (size_t)((void*)end - (void*)base);
		if (line_len > 256) {
			line_len = 256;
		}
		
		// Copy the memory into the buffer (keep in mind the buffer is cleared
		// very time the loop comes around). 
		memcpy(buffer, base, line_len);
		
		if (end) {
			if (buffer[0] == '#' && enable_comments) {
				// Comment - skip line if comments are enabled
			}
			else {
				char *key = memchr(buffer, '=', 256);
				
				// Any line without an '=' is just a comment.
				if (key) {
					key[0] = '\0'; // set '=' to be '\0'
					key++;
					
					size_t vs = strlen(buffer) + 1;
					size_t ks = strlen(key) + 1;
					
					// Using one alloc to save space and time
					char *final = (char *) DgAlloc(vs + ks);
					
					if (!final) {
						printf("Failed to allocate memory for SimpleConfig.\n");
						return (void *) 0;
					}
					
					strcpy(final, buffer);
					strcpy(final + vs, key);
					
					if (_DG_SIMPLE_CONFIG_PARSER_DEBUG) {
						printf("SimpleConfig: Have \"%s\" = \"%s\".\n", final, final+vs);
					}
					
					DgBagSet(&conf->config, final, final + vs);
				}
			}
		}
		
		// Push up the string position
		breadth = breadth + (size_t)((void*)end - (void*)base) + 1;
	}
	
#if defined(_DG_SIMPLE_CONFIG_PARSER_CLOCK)
	__ck = DgTime() - __ck;
	printf("INI Parser Preformance Results:\n\tParsed %d chars in %f seconds.\n\tAbout %f chars per clock cycle.\n", file->size, __ck, (float) file->size / __ck);
#endif
	
	// The file can now be unloaded
	DgUnloadBinaryFile(file);
	
	return conf;
}

void DgConfigPrint(DgConfig *config) {
	/* Prints the contents of a DgConfig file. */
	DgBagPrint(&config->config);
}

void DgConfigFree(DgConfig *config) {
	/* Frees a DgConfig file. */
	for (size_t i = 0; i < config->config.size; i++) {
		DgFree((void *) config->config.key[i]);
	}
	
	DgBagFree(&config->config);
	DgFree(config);
}

DgBag *DgConfigGetBag(DgConfig *config) {
	/* Gets the bag used with a config file. */
	return &config->config;
}
