/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * DEPRECATED: This entire file will be replaced by a more basic format.
 * 
 * Config File Parser and Loader
 */

#include <string.h>
#include <stdbool.h>

#include "../util/bag.h"
#include "../util/alloc.h"
#include "../util/fail.h"
#include "../io/fs.h"
#include "../io/load.h"

#include "config.h"

inline static void tokSet(DgINIToken *tok, DgINILoaderTokenType type, char *dat) {
	tok->type = type;
	tok->data = dat;
}

DgConfig *DgConfigLoad(char* path) {
	path = DgEvalPath(path);
	
	DgLoadBinaryFileInfo *file = DgLoadBinaryFile(path);
	
	// Free path name string
	DgFree(path);
	
	// Token data
	size_t tokens_count = 1;
	DgINIToken *tokens = DgAlloc(sizeof(DgINIToken) * tokens_count);
	
	if (!tokens) {
		DgFail("Failed to allocate memory for INI tokens.\n", -1);
	}
	
	// Tokenise
	{
		// Document data
		size_t length = file->size;
		const char *data = (const char *) file->data;
		
		tokSet((tokens + 0), DG_INI_IGNORE, NULL);
		
		for (size_t i = 0; i < length; i++) {
			char this = data[i];
			
			switch (this) {
				// Note: spaces are a special case
				case '\n':
				case '=':
					// Add setting delmimiter
					
					// Ignore delmimiters before data and extra ones
					// IMPORTANT: check tokens_count FIRST, otherwise we could
					// read from external memory..
					if (tokens_count < 2 || tokens[tokens_count - 1].type == DG_INI_DELIMITER) {
						break;
					}
					
					tokens_count++;
					tokens = DgRealloc(tokens, sizeof(DgINIToken) * tokens_count);
					
					if (!tokens) {
						DgFail("Failed to reallocate memory for INI tokens.\n", -1);
					}
					
					tokSet((tokens + (tokens_count - 1)), DG_INI_DELIMITER, NULL);
					
					break;
				case '[':
				case ']':
					// Add section delimiter
					tokens_count++;
					tokens = DgRealloc(tokens, sizeof(DgINIToken) * tokens_count);
					
					if (!tokens) {
						DgFail("Failed to reallocate memory for INI tokens.\n", -1);
					}
					
					tokSet((tokens + (tokens_count - 1)), DG_INI_SECTION, NULL);
					
					break;
				default:
					// Create a new name if not already a name
					if (tokens[tokens_count - 1].type != DG_INI_NAME) {
						tokens_count++;
						tokens = DgRealloc(tokens, sizeof(DgINIToken) * tokens_count);
						
						if (!tokens) {
							DgFail("Failed to reallocate memory for INI tokens.\n", -1);
						}
						
						tokens[tokens_count - 1].data = DgAlloc(sizeof(char) * 2);
						
						if (!tokens[tokens_count - 1].data) {
							DgFail("Failed to allocate memory for INI string name.\n", -1);
						}
						
						tokens[tokens_count - 1].type = DG_INI_NAME;
						tokens[tokens_count - 1].data[0] = this;
						tokens[tokens_count - 1].data[1] = '\0';
					}
					// Otherwise, add to this name
					else {
						size_t datalen = strlen(tokens[tokens_count - 1].data) + 1;
						
						tokens[tokens_count - 1].data = DgRealloc(
							tokens[tokens_count - 1].data, 
							++datalen);
						
						tokens[tokens_count - 1].data[datalen - 2] = this;
						tokens[tokens_count - 1].data[datalen - 1] = '\0';
					}
					
					break;
			}
		}
	}
	
	// The file can now be unloaded
	DgUnloadBinaryFile(file);
	
	DgConfig *result = (DgConfig *) DgAlloc(sizeof(DgConfig));
	
	if (!result) {
		DgFail("Failed to allocate memory for DgConfig struct.\n", -1);
	}
	
	memset(result, 0, sizeof(DgConfig));
	
	// Parse into DgBag tree
	{
		/*
		for (size_t i = 0; i < tokens_count; i++) {
			char *c = "";
			
			switch (tokens[i].type) {
				case DG_INI_IGNORE:
					printf("ini: TOK_IGNORE\n");
					break;
				case DG_INI_SECTION:
					printf("ini: TOK_SECT\n");
					break;
				case DG_INI_DELIMITER:
					printf("ini: TOK_DELIMIT\n");
					break;
				case DG_INI_NAME:
					if (tokens[i].data) {
						c = tokens[i].data;
					}
					
					printf("ini: TOK_NAME '%s'\n", c);
					break;
			}
		}
		*/
		
		bool isSection = false;
		bool isKey = true;
		bool isValue = false;
// 		bool isComment = false;
		DgBag *current_bag;
		char *next_key;
		
		for (size_t i = 0; i < tokens_count; i++) {
			switch (tokens[i].type) {
				case DG_INI_IGNORE:
					break;
				case DG_INI_SECTION:
					isSection = !isSection;
					break;
				case DG_INI_DELIMITER:
					if (tokens[i - 1].type == DG_INI_NAME && tokens[i - 1].data[0] != ';') {
						isValue = !isValue;
						isKey = !isKey;
					}
					break;
				case DG_INI_NAME:
					if (isSection) {
						// Makes the section titles and bags
						if (!result->sections) {
							result->sections = (char **) DgAlloc(sizeof(char *) * 1);
						}
						else {
							result->sections = (char **) DgRealloc(result->sections, sizeof(char *) * (result->size + 1));
						}
						
						if (!result->sections) {
							DgFail("Memory allocation failure for INI sections.\n", -1);
						}
						
						result->sections[result->size] = tokens[i].data;
						
						if (!result->configs) {
							result->configs = (DgBag *) DgAlloc(sizeof(DgBag) * 1);
						}
						else {
							result->configs = (DgBag *) DgRealloc(result->sections, sizeof(DgBag) * (result->size + 1));
						}
						
						if (!result->configs) {
							DgFail("Memory allocation failure for INI k/v pairs.\n", -1);
						}
						
						result->configs[result->size] = DgBagInit();
						current_bag = (result->configs + result->size);
						
						result->size++;
					}
					else if (isKey) {
						next_key = tokens[i].data;
					}
					else if (isValue) {
						// Adds properties to the bags
						DgBagSet(current_bag, next_key, tokens[i].data);
					}
					else {
						printf("INI Parser: Bad INI file.\n");
						return (void *) 0;
					}
			}
		}
	}
	
	DgFree(tokens);
	
	return result;
}

void DgConfigPrint(DgConfig *config) {
	for (size_t i = 0; i < config->size; i++) {
		printf("%s\n", config->sections[i]);
		DgBagPrint(&config->configs[i]);
	}
}

void DgConfigFree(DgConfig *config) {
	for (size_t i = 0; i < config->size; i++) {
		DgFree(config);
	}
	
	DgFree(config->sections);
	DgFree(config);
}
