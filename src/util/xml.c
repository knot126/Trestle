/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Decent Games XML Parser
 */

#include "../util/alloc.h"
#include "../io/fs.h"

#include "xml.h"

void DgXMLDocumentLoad(DgXMLDocument *doc, const char *path) {
	char *real_path = DgEvalPath((char *) path);
	
	DgFree(real_path);
}

