/*	
main_linux.cpp -  Linux command-line entry point for the columbia optimizer
Modified for Linux compatibility
Columbia Optimizer Framework

  A Joint Research Project of Portland State University 
  and the Oregon Graduate Institute
  Directed by Leonard Shapiro and David Maier
  Supported by NSF Grants IRI-9610013 and IRI-9619977
*/

#include "stdafx.h"
#include "cat.h"
#include "cm.h"
#include "tasks.h"
#include "physop.h"
#include "global.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

// Dummy output window for Linux
DummyOutputWindow dummyWindow;

// Global pointer to output window (required by other modules)
DummyOutputWindow* OutputWindow = &dummyWindow;

// Forward declaration
void Optimizer();

int main(int argc, char* argv[]) {
	printf("Columbia Optimizer Framework - Linux Port\n");
	printf("==========================================\n\n");
	
	// Get current directory as AppDir
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		AppDir = cwd;
	} else {
		AppDir = ".";
	}
	
	// Parse command line arguments
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-catalog") == 0 && i + 1 < argc) {
			CatFile = argv[++i];
		} else if (strcmp(argv[i], "-query") == 0 && i + 1 < argc) {
			SQueryFile = argv[++i];
			RadioVal = 1; // Single query mode
		} else if (strcmp(argv[i], "-batch") == 0 && i + 1 < argc) {
			BQueryFile = argv[++i];
			RadioVal = 0; // Batch query mode
		} else if (strcmp(argv[i], "-cm") == 0 && i + 1 < argc) {
			CMFile = argv[++i];
		} else if (strcmp(argv[i], "-ruleset") == 0 && i + 1 < argc) {
			RSFile = argv[++i];
		} else if (strcmp(argv[i], "-trace") == 0) {
			FileTrace = true;
		} else if (strcmp(argv[i], "-notrace") == 0) {
			FileTrace = false;
		} else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-h") == 0) {
			printf("Usage: %s [options]\n", argv[0]);
			printf("Options:\n");
			printf("  -catalog <file>   Catalog file (default: catalog)\n");
			printf("  -query <file>     Single query file (default: query)\n");
			printf("  -batch <file>     Batch query file (default: bquery)\n");
			printf("  -cm <file>        Cost model file (default: cm)\n");
			printf("  -ruleset <file>   Rule set file (default: ruleset)\n");
			printf("  -trace            Enable file tracing\n");
			printf("  -notrace          Disable file tracing\n");
			printf("  -help, -h         Show this help message\n");
			return 0;
		}
	}
	
	// Set default mode if not specified
	if (RadioVal == -1) {
		RadioVal = 1; // Default to single query mode
	}
	
	printf("Configuration:\n");
	printf("  Catalog file: %s\n", CatFile.c_str());
	printf("  Cost model file: %s\n", CMFile.c_str());
	printf("  Rule set file: %s\n", RSFile.c_str());
	if (RadioVal == 1) {
		printf("  Query file: %s\n", SQueryFile.c_str());
	} else {
		printf("  Batch query file: %s\n", BQueryFile.c_str());
	}
	printf("  File trace: %s\n\n", FileTrace ? "enabled" : "disabled");
	
	// Run the optimizer
	try {
		Optimizer();
		printf("\nOptimization completed successfully.\n");
		return 0;
	} catch (const char* error) {
		fprintf(stderr, "Error: %s\n", error);
		return 1;
	} catch (...) {
		fprintf(stderr, "Unknown error occurred.\n");
		return 1;
	}
}
