/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Tests
 */

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "util/log.h"
#include "util/json.h"
#include "util/fs.h"

typedef struct {
	DgFileStream *output;
} Tester;

static const char * test_DgJSON(void) {
	DgLog(DG_LOG_INFO, "JSON test...");
	
	DgJSONValue val;
	const char a[] = "{\"test\": 6.30,\n\t\"abc\": \"def\", \"q\": [0, 0, 0],\n\t\"np\": null,\n\t\"a-true\": true, \"a-false\": false\n, \"objec\": {}, \"arre\": [], \"thing\": {\"ssss\": 534}}";
	
	if (DgJSONParse(&val, sizeof a - 1, a)) {
		return "Failed to load JSON document.";
	}
	
	DgJSONValuePrint(&val);
	
	DgJSONValueFree(&val);
	
	return "Okay";
}

static void run_test(Tester *t, const char * (*func)(void), char *what) {
	// Call the test function
	const char * message = (func)();
	
	// Write debug output
	DgFileStreamWriteString(t->output, what);
	DgFileStreamWriteString(t->output, ": ");
	DgFileStreamWriteLine(t->output, (char *) message);
}

void do_all_tests(void) {
	DgLog(DG_LOG_INFO, "Starting in test mode...");
	
	Tester t;
	// Open File Stream
	t.output = DgFileStreamOpen("test-results.txt", "wb");
	
	if (!t.output) {
		DgLog(DG_LOG_FATAL, "Failed to open file to write test results.");
		return;
	}
	
	// Decent Games Libraries
	run_test(&t, test_DgJSON, "Test-Json");
	
	// Close File
	DgFileStreamClose(t.output);
}
