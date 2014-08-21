/*
 * Will Korteland
 * 21st August 2014
 * Parsing module for compiler
 */

#ifndef _LEXER_H_
#define _LEXER_H_

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

enum tokens {
    LBRACKET,
    RBRACKET,
    LPAREN,
    RPAREN,
    LCURLY,
    RCURLY,
    DOT,
    S_DEREF,
    INCREMENT,
    DECREMENT,
    BIT_AND,
    STAR,
    PLUS,
    MINUS,
    TILDE,
    NOT,
    DIV,
    MOD,
    LSHIFT,
    RSHIFT,
    LT,
    GT,
    LTE,
    GTE,
    EQEQ,
    NEQ,
    XOR,
    BIT_OR,
    LOG_AND,
    LOG_OR,
    QMARK,
    COLON,
    SEMICOLON,
    ELLIPSE,
    EQ,
    MULT_EQ,
    DIV_EQ,
    MOD_EQ,
    PLUS_EQ,
    MINUS_EQ,
    LSHIFT_EQ,
    RSHIFT_EQ,
    AND_EQ,
    XOR_EQ,
    OR_EQ,
    COMMA,
    HASH,
    CONCAT,
    KEYWORD,
    IDENTIFIER,
    ERROR
};

static char* operators[] = {
    "[",
    "]",
    "(",
    ")",
    "{",
    "}",
    ".",
    "->",
    "++",
    "--",
    "&",
    "*",
    "+",
    "-",
    "~",
    "!",
    "/",
    "%",
    "<<",
    ">>",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "^",
    "|",
    "&&",
    "||",
    "?",
    ":",
    ";",
    "...",
    "=",
    "*=",
    "/=",
    "%=",
    "+=",
    "-=",
    "<<=",
    ">>=",
    "&=",
    "^=",
    "|=",
    ",",
    "#",
    "##",
    "KEYWORD",
    "IDENTIFIER",
    "@"
};

int parse(const char *fname);

#endif // _LEXER_H_
