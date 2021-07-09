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
#include "log.h"

#include "xml.h"

typedef struct {
	enum {
		DG_XML_UNKNOWN    = 0,
		DG_XML_TAG_NAME   = 1,
		DG_XML_TAG_END    = 2,
		DG_XML_TEXT_START = 3,
		DG_XML_NAME       = 4,
		DG_XML_STRING     = 5,
		DG_XML_ASSOC      = 6,
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
	if ((seq[0] == '<') && (seq[1] == '/')) 
		{ return 2; }
	else if (*seq == '<') 
		{ return 1; }
	else 
		{ return 0; }
}

static int is_tag_end(const char * const seq) {
	/**
	 * Returns 0 if not any, 1 if normal close, 2 for self-closing
	 */
	if ((seq[0] == '/') && (seq[1] == '>'))
		{ return 2; }
	else if (*seq == '>')
		{ return 1; }
	else
		{ return 0; }
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

static bool xml_strnext(char *str, char *what) {
	/**
	 * Find if str starts with what.
	 */
	
	int len = strlen(what);
	int k = 0;
	
	while (*str != '\0' && *what != '\0' && *(str++) == *(what++)) {
		k++;
	}
	
	return (len == k);
}

typedef struct {
	char *key;
	char value;
} Dg_XMLAssocEscape;

static char *xml_escape_and_free(char *string) {
	/**
	 * Escape an XML string, returning unescaped string if there is a fault
	 */
	
	const Dg_XMLAssocEscape EscapeValues[] = {
		{"amp", '&'},
		{"apos", '\''},
		{"quot", '"'},
		{"lt", '<'},
		{"gt", '>'},
		{"", '~'}, // invalid escape sequence
	};
	
	size_t len = strlen(string);
	size_t lenallocate = len + 1;
	
	// Calculate the length of the unescaped string
	for (size_t i = 0; i < len; i++) {
		if (string[i] == '&') {
			while (string[i++] != ';') {
				if (string[i] == '\0') { // something went wrong, just return unescaped
					return string;
				}
				
				lenallocate--;
			}
		}
	}
	
	// Check if any processing is needed at all
	if (len == (lenallocate - 1)) {
		return string;
	}
	
	// Allocate new string
	char *new = (char *) DgAlloc(sizeof *new * lenallocate);
	
	if (!new) {
		return string;
	}
	
	new[lenallocate - 1] = '\0';
	
	// Fill in the new string with values
	for (size_t i = 0, j = 0; i < len; i++, j++) {
		// Check for escape
		if (string[i] == '&') {
			++i;
			
			// Find what the escape is assocaited with...
			for (size_t k = 0; k < sizeof(EscapeValues); k++) {
				if (xml_strnext(&string[i], EscapeValues[k].key)) {
					// Replace escape with char
					new[j] = EscapeValues[k].value;
					while (string[i] != ';') {
						i++;
					};
					break;
				}
			}
		}
		else {
			new[j] = string[i];
		}
	}
	
	// Free old string
	DgFree(string);
	
	return new;
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
	{
	for (size_t i = 0; i < content_size; i++) {
		printf("INTO: %c\n", content[i]);
		
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
			
			// NOTE: This fixes an issue where we increment too far on the end of the tag.
			if (is_tag_end(&content[i])) {
				i--;
			}
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
			
			i++;
			size_t start = i;
			while (is_tag_start(&content[i]) == 0) {
				i++;
			}
			
			char *t = (i - start) ? DgStrdupl(&content[start], i - start) : NULL;
			
			tokens[token_count - 1].type = DG_XML_TEXT_START;
			tokens[token_count - 1].text = t;
			
			// So we do not go past the start of the next tag.
			i--;
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
			while (!is_whitespace(&content[i]) && !is_assoc(&content[i]) && !is_tag_start(&content[i])) {
				i++;
			}
			tokens[token_count - 1].text = DgStrdupl(&content[start], i - start);
			
			// NOTE: This fixes an issue where we increment too far on the equal sign.
			if (is_assoc(&content[i])) {
				i--;
			}
		}
	}
	}
	
	/*
	 * Pre-parser steps
	 */
	memset(doc, 0, sizeof *doc);
	
// 	for (size_t i = 0; i < token_count; i++) {
// 		printf("%.3d   %d   '%s'\n", i, tokens[i].type, tokens[i].text);
// 	}
	
	/**
	 * XML Parser loop
	 */
	uint32_t depth = 0;
	DgXMLNode *current = (DgXMLNode *) doc;
	
	for (size_t i = 0; i < token_count; i++) {
		// Sync to starting node
		current = doc;
		for (size_t i = 1; i < depth; i++) {
			current = &current->sub[current->sub_count - 1];
		}
		
		// If we find a new tag
		if (tokens[i].type == DG_XML_TAG_NAME) {
			// New subelement that is not the document root
			if (depth > 0) {
				current->sub = DgRealloc(current->sub, sizeof *current->sub * ++current->sub_count);
				
				if (!current->sub) {
					DgLog(DG_LOG_ERROR, "[XML Parser] Failed to allocate memory for subnodes.\n");
					return 3;
				}
				
				current = &current->sub[current->sub_count - 1];
				memset(current, 0, sizeof(DgXMLNode));
			}
			
			depth++;
			current->name = tokens[i].text;
			
			// Peek the next token to see if we have attributes
			// If it is name 'name,' we certianly have attributes
			while ((i + 1) < token_count && tokens[i + 1].type == DG_XML_NAME) {
				i++;
				
				char *key = tokens[i].text;
				
				// Check that we have enough tokens left and if the next token
				// is what we expect (an assocaitor). If not, then error.
				if ((i + 1) < token_count && tokens[i + 1].type == DG_XML_ASSOC) {
					i++;
					
					if ((i + 1) < token_count && tokens[i + 1].type == DG_XML_STRING) {
						i++;
						
						char *value = tokens[i].text;
						
						current->attrib_count++;
						current->attrib = DgRealloc(current->attrib, current->attrib_count * sizeof(DgXMLPair));
						
						if (!current->attrib) {
							DgLog(DG_LOG_ERROR, "[XML Parser] Failed to allocate memory for attributes.\n");
							return 5;
						}
						
						value = xml_escape_and_free(value);
						
						if (!current->attrib) {
							DgLog(DG_LOG_ERROR, "[XML Parser] Failed to allocate memory for escaped string.\n");
							return 6;
						}
						
						current->attrib[current->attrib_count - 1].key = key;
						current->attrib[current->attrib_count - 1].value = value;
					}
					else { // End of doc or did not get string
						DgLog(DG_LOG_ERROR, "[XML Parser] Expected string, got other type or end of document.\n");
						return 4;
					}
				}
				else { // End of doc or did not get assoc
					DgLog(DG_LOG_ERROR, "[XML Parser] Expected assocatior, got other type or end of document.\n");
					return 2;
				}
			}
		}
		
		// Node text
		else if (tokens[i].type == DG_XML_TEXT_START) {
			current->text = tokens[i].text;
		}
		
		// Closing tags
		else if (tokens[i].type == DG_XML_TAG_END) {
			depth--;
		}
	}
	
	// Free the token list
	DgFree(tokens);
	
	if (depth != 0) {
		DgLog(DG_LOG_ERROR, "[XML Parser] The depth after loading the doucment is not zero; could the document not have its tags closed properly?\n");
		return 1;
	}
	
	return 0;
}

uint32_t DgXMLLoadString(DgXMLNode *doc, const char * const str) {
	/**
	 * Load a NULL-terminated string that contains an XML document.
	 */
	
	return DgXMLParse(doc, strlen(str), str);
}

uint32_t DgXMLLoad(DgXMLNode *doc, const char * const path) {
	/**
	 * Load an XML document from a file into a basic parsed tree
	 */
	char *tmp_content;
	
	// Load the file
	
	char *real_path = (char *) DgEvalPath((char *) path);
	
	if (!real_path) {
		DgLog(DG_LOG_ERROR, "[XML Parser] Could not load file at %s.\n", real_path);
		return 1;
	}
	
	DgFileStream *stream = DgFileStreamOpen(real_path, "rb");
	
	if (!stream) {
		DgLog(DG_LOG_ERROR, "[XML Parser] Could not load file at %s.\n", real_path);
		return 1;
	}
	
	DgFree(real_path);
	
	size_t doc_size = DgFileStreamLength(stream);
	tmp_content = DgAlloc(doc_size);
	
	if (!tmp_content) {
		DgLog(DG_LOG_ERROR, "[XML Parser] Could not allocate memory to load file.\n");
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
	
	if (doc->text) {
		for (uint32_t i = 0; i < depth; i++) {
			printf("\t");
		}
		
		printf("\033[1;33mText\033[0m: \033[0;95m\"%s\"\033[0m\n", doc->text);
	}
	else {
		for (uint32_t i = 0; i < depth; i++) {
			printf("\t");
		}
		printf("(\033[0;33mno text\033[0m)\n");
	}
	
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
	/**
	 * Get the value assocaited with an attribute on an XML node, returning 
	 * fallback if not available. 
	 */
	
	char *value = fallback;
	
	for (size_t i = 0; i < node->attrib_count; i++) {
		if (!strcmp(node->attrib[i].key, key)) {
			value = node->attrib[i].value;
			break;
		}
	}
	
	return value;
}

inline const char *DgXMLNodeGetName(DgXMLNode *node) {
	/**
	 * Get an XML node's name.
	 */
	
	return node->name;
}

inline const char *DgXMLNodeGetText(DgXMLNode *node) {
	/**
	 * Get an XML node's text.
	 */
	
	return node->text;
}

inline size_t DgXMLNodeGetAttribCount(DgXMLNode *node) {
	/**
	 * The the count of attributes in an XML node.
	 */
	
	return node->attrib_count;
}

inline DgXMLPair *DgXMLNodeGetAttrib(DgXMLNode *node, size_t index) {
	/**
	 * Get the key/value pair of the attribute at index.
	 */
	
	return &node->attrib[index];
}

inline size_t DgXMLNodeGetSubCount(DgXMLNode *node) {
	/**
	 * The the count of subnodes in an XML node.
	 */
	
	return node->sub_count;
}

inline DgXMLNode *DgXMLNodeGetSub(DgXMLNode *node, size_t index) {
	/**
	 * Get the key/value pair of the attribute at index.
	 */
	
	return &node->sub[index];
}
