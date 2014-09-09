/*
 * Will Korteland
 * 21st August 2014
 * Driver module for compiler
 */

#include <stdio.h>
#include <stdlib.h>

#include "source/c/lexer.h"

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./compile <FILE.C>\n");
        return EXIT_FAILURE;
    }

    if (lexerInit(argv[1])) {
        printf("Failed to compile source file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    Token temp;
    temp = nextToken();
    while (temp.kind != END) {
        printf("Token: %d on line %d, position %d: %s\n", temp.kind, temp.pos.line, temp.pos.linePos, temp.spelling.spelling);
        temp = nextToken();
    }
    printf("Token: %d on line %d, position %d: %s\n", temp.kind, temp.pos.line, temp.pos.linePos, temp.spelling.spelling);

    return EXIT_SUCCESS;
}
