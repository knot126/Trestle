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
#include "util/fs.h"
#include "util/bezsurfa.h"

typedef struct {
	DgFileStream *output;
} Tester;

static const char * test_DgAvancedMathFuncs(void) {
	// test advanced maths functions
	
	DgLog(DG_LOG_INFO, "Test maths functions...");
	
	float r = DgBersteinPolynomial(1.0, 0.0, 0.5);
	
	if (r != 0.5) {
		return "B 1,0 (0.5) != 0.5, should though";
	}
	
	r = DgBersteinPolynomial(1.0, 1.0, 0.5);
	
	if (r != 0.5) {
		return "B 1,1 (0.5) != 0.5, should though";
	}
	
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
	run_test(&t, test_DgAvancedMathFuncs, "Test-Maths");
	
	// Close File
	DgFileStreamClose(t.output);
}
