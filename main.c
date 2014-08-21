/*
 * Will Korteland
 * 21st August 2014
 * Driver module for compiler
 */

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./compile <FILE.C>\n");
	return EXIT_FAILURE;
    }

    if (parse(argv[1])) {
	printf("Failed to compile source file: %s\n", argv[1]);
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
