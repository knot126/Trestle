/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser A2
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "alloc.h"
#include "str.h"
#include "fs.h"

#include "xml.h"

/**
 * Data Types
 */

typedef struct {
	enum {
		DG_XML_UNKNOWN,
		DG_XML_TAG_NAME,
		DG_XML_TAG_END,
		DG_XML_TEXT_START,
		DG_XML_NAME,
		DG_XML_STRING,
		DG_XML_ASSOC,
	} type;
	char *text;
} Dg_XMLParseToken;

/**
 * Utility Functions
 */

static void expand_tokens(Dg_XMLParseToken **data, size_t * const current_size) {
	*current_size = *current_size + 1;
	*data = DgRealloc(*data, *current_size * sizeof(Dg_XMLParseToken));
}

static bool is_whitespace(const char * const seq) {
	return (*seq == ' ' || *seq == '\t' || *seq == '\n' || *seq == '\r');
}

static int is_tag_start(const char * const seq) {
	/**
	 * Returns 0 if none, 1 if normal open, 2 if close tag
	 */
	if ((seq[0] == '<') && (seq[1] == '/')) { return 2; }
	return (*seq == '<');
}

static int is_tag_end(const char * const seq) {
	/**
	 * Returns 0 if not any, 1 if normal close, 2 for self-closing
	 */
	if ((seq[0] == '/') && (seq[1] == '>')) { return 2; }
	return (*seq == '>');
}

static bool is_assoc(const char * const seq) {
	return (*seq == '=');
}

static bool is_string_term(const char * const seq) {
	return (*seq == '"');
}

static bool is_comment_start(const char * const seq) {
	return (seq[0] == '<' && seq[1] == '!' && seq[2] == '-' && seq[3] == '-');
}

static bool is_comment_end(const char * const seq) {
	return (seq[0] == '-' && seq[1] == '-' && seq[2] == '>');
}

/**
 * Main Parsing and Loading Functions
 * ==================================
 * 
 * These are some parser functions for simpily loading the document into memory.
 */

uint32_t DgXMLParse(DgXMLNode * const doc, const uint32_t content_size, const char * const content) {
	/**
	 * <summary>Parse an XML document (version 2)</summary>
	 * <input name="doc">The document node that the document will be loaded into.</input>
	 * <input name="content_size">The size of the buffer.</input>
	 * <input name="content">The fully loaded buffer of the document.</input>
	 */
	
	// Lexer Nonlocals
	Dg_XMLParseToken *tokens = NULL;
	size_t token_count = 0;
	
	/**
	 * Main tokeniser loop
	 */
	for (size_t i = 0; i < content_size; i++) {
		if (is_whitespace(&content[i])) {
			continue;
		}
		
		else if (is_comment_start(&content[i])) {
			while (!is_comment_end(&content[i])) {
				i++;
			}
			i += 3;
		}
		
		else if (is_tag_start(&content[i]) == 1) {
			expand_tokens(&tokens, &token_count);
			
			tokens[token_count - 1].type = DG_XML_TAG_NAME;
			
			i++;
			size_t start = i;
			while (!is_whitespace(&content[i]) && !is_tag_end(&content[i])) {
				i++;
			}
			tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
		}
		
		else if (is_assoc(&content[i])) {
			expand_tokens(&tokens, &token_count);
			
			tokens[token_count - 1].type = DG_XML_ASSOC;
			tokens[token_count - 1].text = NULL;
		}
		
		else if (is_string_term(&content[i])) {
			expand_tokens(&tokens, &token_count);
			
			tokens[token_count - 1].type = DG_XML_STRING;
			
			i++;
			size_t start = i;
			while (!is_string_term(&content[i])) {
				i++;
			}
			tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
		}
		
		else if (is_tag_end(&content[i]) == 1) {
			expand_tokens(&tokens, &token_count);
			
			tokens[token_count - 1].type = DG_XML_TEXT_START;
			tokens[token_count - 1].text = NULL;
		}
		
		else if (is_tag_end(&content[i]) == 2 || is_tag_start(&content[i]) == 2) {
			expand_tokens(&tokens, &token_count);
			
			tokens[token_count - 1].type = DG_XML_TAG_END;
			tokens[token_count - 1].text = NULL;
			
			// skip this portion of the document
			while (is_tag_end(&content[i]) == 0) {
				i++;
			}
		}
		
		else {
			expand_tokens(&tokens, &token_count);
			
			tokens[token_count - 1].type = DG_XML_NAME;
			
			size_t start = i;
			while (!is_whitespace(&content[i]) && !is_assoc(&content[i])) {
				i++;
			}
			tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
			
			// NOTE: This fixes an issue where we increment too far on the equal sign.
			if (is_assoc(&content[i])) {
				i--;
			}
		}
	}
	
	//printf("Took %d iterations to parse document.\n", icount);
	
	/**
	 * Debug: Print out all the tokens in a nice format.
	 */
#if 0
	for (size_t i = 0; i < token_count; i++) {
		printf("[ %.3d ] %d -> %s\n", i, tokens[i].type, tokens[i].text);
	}
#endif
	
	/*
	 * Pre-parser steps
	 */
	memset(doc, 0, sizeof(DgXMLNode));
	
	/**
	 * XML Parser loop
	 */
	uint32_t depth = 0;
	DgXMLNode *current = (DgXMLNode *) doc;
	
	for (size_t i = 0; i < token_count; i++) {
		//printf("================================================\n");
		//DgXMLPrintNode(doc, 0);
		
		// Sync the current node to the starting node
		current = doc;
		for (size_t i = 1; i < depth; i++) {
			current = &current->sub[current->sub_count - 1];
		}
		
		// If we find a new tag
		if (tokens[i].type == DG_XML_TAG_NAME) {
			// New subelement that is not the document root
			if (depth > 0) {
				//printf("current->sub_count is 0, current is %x\n", current);
				current->sub_count++;
				
				//printf("DgRealloc(<%x>, %d)\n", current->sub, current->sub_count);
				current->sub = DgRealloc(current->sub, current->sub_count * sizeof(DgXMLNode));
				
				if (!current->sub) {
					printf("\033[1;33mWarning:\033[0m XML Parser Error: Failed to allocate memory for subnodes.\n");
					return 3;
				}
				
				current = &current->sub[current->sub_count - 1];
				memset(current, 0, sizeof(DgXMLNode));
			}
			
			depth++;
			current->name = tokens[i].text;
			
			//printf("%d: %s\n", depth, current->name);
			
			// Peek the next token to see if we have attributes
			// If it is name 'name,' we certinaly have attributes
			while ((i + 1) < token_count && tokens[i + 1].type == DG_XML_NAME) {
				i++;
				
				char *key = tokens[i].text;
				
				// Check that we have enough tokens left then check if assoc
				if ((i + 1) < token_count && tokens[i + 1].type == DG_XML_ASSOC) {
					i++;
					
					if ((i + 1) < token_count && tokens[i + 1].type == DG_XML_STRING) {
						i++;
						
						char *value = tokens[i].text;
						
						current->attrib_count++;
						current->attrib = DgRealloc(current->attrib, current->attrib_count * sizeof(DgXMLPair));
						
						if (!current->attrib) {
							printf("\033[1;33mWarning:\033[0m XML Parser Error: Failed to allocate memory for attributes.\n");
							return 5;
						}
						
						current->attrib[current->attrib_count - 1].key = key;
						current->attrib[current->attrib_count - 1].value = value;
					}
					else {
						printf("\033[1;33mWarning:\033[0m XML Parser Error: Expected string, got other type or end of document.\n");
						return 4;
					}
				}
				else {
					printf("\033[1;33mWarning:\033[0m XML Parser Error: Expected assocatior, got other type or end of document.\n");
					return 2;
				}
			}
		}
		
		// Closing tags
		if (tokens[i].type == DG_XML_TAG_END) {
			depth--;
		}
	}
	
	// Free the token list
	DgFree(tokens);
	
	if (depth != 0) {
		printf("\033[1;33mWarning:\033[0m XML Parser Error: The depth after loading the doucment is not zero; could the document not have its tags closed properly?\n");
		return 1;
	}
	
	return 0;
}

uint32_t DgXMLLoad(DgXMLNode *doc, const char * const path) {
	/**
	 * Load an XML document from a file into a basic parsed tree
	 */
	char *tmp_content;
	
	// Load the file
	
	char *real_path = (char *) DgEvalPath((char *) path);
	
	if (!real_path) {
		printf("Warning: XML Parser Error: Could not load file at %s.\n", real_path);
		return 1;
	}
	
	DgFileStream *stream = DgFileStreamOpen(real_path, "rb");
	
	if (!stream) {
		printf("Warning: XML Parser Error: Could not load file at %s.\n", real_path);
		return 1;
	}
	
	DgFree(real_path);
	
	size_t doc_size = DgFileStreamLength(stream);
	tmp_content = DgAlloc(doc_size);
	
	if (!tmp_content) {
		printf("Warning: XML Parser Error: Could not allocate memory to load file.\n");
		return 1;
	}
	
	DgFileStreamRead(stream, doc_size, tmp_content);
	DgFileStreamClose(stream);
	
	const char * const content = tmp_content;
	
	uint32_t stat = DgXMLParse(doc, doc_size, content);
	
	DgFree((void *) content);
	
	return stat;
}

void DgXMLNodeFree(DgXMLNode *node) {
	/**
	 * Free an XML node or document
	 */
	if (node->name) {
		DgFree(node->name);
	}
	
	if (node->attrib) {
		DgFree(node->attrib);
	}
	
	if (node->sub) {
		for (size_t i = 0; i < node->sub_count; i++) {
			DgXMLNodeFree(&node->sub[i]);
		}
	}
	
	if (node->text) {
		DgFree(node->text);
	}
}

void DgXMLPrintNode(const DgXMLNode * const doc, uint32_t depth) {
	/**
	 * Print out the documents to a node, with formatting and colour.
	 */
	for (uint32_t i = 0; i < depth; i++) {
		printf("\t");
	}
	
	printf("\033[1;32m%s\033[0m:\n", doc->name);
	depth++;
	
	if (doc->attrib) {
		for (size_t j = 0; j < doc->attrib_count; j++) {
			for (uint32_t i = 0; i < depth; i++) {
				printf("\t");
			}
			
			printf("\033[0;34m%s\033[0m = \033[0;31m%s\033[0m\n", doc->attrib[j].key, doc->attrib[j].value);
		}
	}
	else {
		for (uint32_t i = 0; i < depth; i++) {
			printf("\t");
		}
		printf("(\033[0;33mno attributes\033[0m)\n");
	}
	
	if (doc->sub) {
		for (size_t j = 0; j < doc->sub_count; j++) {
			DgXMLPrintNode(&doc->sub[j], depth);
		}
	}
	else {
		for (uint32_t i = 0; i < depth; i++) {
			printf("\t");
		}
		printf("(\033[0;33mno subnodes\033[0m)\n");
	}
}

char *DgXMLGetAttrib(DgXMLNode *node, const char * const restrict key, char *fallback) {
	char *value = fallback;
	
	for (size_t i = 0; i < node->attrib_count; i++) {
		if (!strcmp(node->attrib[i].key, key)) {
			value = node->attrib[i].value;
			break;
		}
	}
	
	return value;
}
