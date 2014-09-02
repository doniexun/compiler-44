/*
 * Will Korteland
 * 21st August 2014
 * Parsing module for compiler
 */

/*
 * TODO:
 * source positions
 * literals - ll/LL/l/L/u/U/f/F prefixes/suffixes
 * more comprehensive error coverage
 * likely more stuff in the spec
 */

#include <stdlib.h>

#include "lexer.h"

char accept(void);
size_t fsize(const char *fname);
int isKeyword(const char *name, int len);
int lexDecimal(void);
int lexHexadecimal(void);
void strList_add(char c);
void strList_copy(char *c);
void strList_free(void);

typedef struct _strNode {
    char c;
    struct _strNode *next;
} StrNode;

static FILE *f;
static char currentChar;
static int i, j, strMax, newline_at_end;
static size_t len;
static Token invalid;
static SourcePos curPos;
static char *curSpelling;
static StrNode *str_start, *str_end;

int lexerInit(const char *fname) {
    f = fopen(fname, "r");

    if (!f) {
        printf("Error: couldn't open source file\n");
        return -1;
    }

    // initialise bookeeping variables
    len = fsize(fname);
    i = 0;
    j = 0;
    strMax = 0;
    newline_at_end = 0;
    curSpelling = NULL;
    str_start = NULL;
    str_end = NULL;
    accept();

    // initialise error token
    invalid.kind = ERROR;
    //invalid.spelling = ""; // TODO
    invalid.pos = curPos;

    // initialise current position
    curPos.startLine = 0;
    curPos.endLine = 0;
    curPos.lineStartPos = 0;
    curPos.lineEndPos = 0;

    return 0;
}

Token nextToken(void) {
    static Token result;

    while (currentChar != EOF) {
        newline_at_end = 0;
        
        if (curSpelling) {
            free(curSpelling);
            curSpelling = NULL;
        }

        switch (currentChar) {
        case '[':
            result.kind = LBRACKET;
            i++;
            accept();
            break;
        case ']':
            result.kind = RBRACKET;
            i++;
            accept();
            break;
        case '(':
            result.kind = LPAREN;
            i++;
            accept();
            break;
        case ')':
            result.kind = RPAREN;
            i++;
            accept();
            break;
        case '{':
            result.kind = LCURLY;
            i++;
            accept();
            break;
        case '}':
            result.kind = RCURLY;
            i++;
            accept();
            break;
        case '.':
            accept();
            switch (currentChar) {
            case '.':
                accept();
                switch (currentChar) {
                case '.':
                    result.kind = ELLIPSE;
                    i++;
                    accept();
                    break;
                default:
                    result.kind = ERROR;
                    i++;
                    break;
                }
            default:
                result.kind = DOT;
                i++;
                break;
            }
            break;
        case '-':
            accept();
            switch (currentChar) {
            case '>':
                result.kind = S_DEREF;
                i++;
                accept();
                break;
            case '-':
                result.kind = DECREMENT;
                i++;
                accept();
                break;
            case '=':
                result.kind = MINUS_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = MINUS;
                i++;
                break;
            }
            break;
        case '+':
            accept();
            switch (currentChar) {
            case '+':
                result.kind = INCREMENT;
                i++;
                accept();
                break;
            case '=':
                result.kind = PLUS_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = PLUS;
                i++;
                break;
            }
            break;
        case '&':
            accept();
            switch (currentChar) {
            case '&':
                result.kind = LOG_AND;
                i++;
                accept();
                break;
            case '=':
                result.kind = AND_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = BIT_AND;
                i++;
                break;
            }
            break;
        case '*':
            accept();
            switch (currentChar) {
            case '=':
                result.kind = MULT_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = STAR;
                i++;
                break;
            }
            break;
        case '~':
            result.kind = TILDE;
            i++;
            accept();
            break;
        case '!':
            accept();
            switch (currentChar) {
            case '=':
                result.kind = NEQ;
                i++;
                accept();
                break;
            default:
                result.kind = NOT;
                i++;
                break;
            }
            break;
        case '/':
            accept();
            switch (currentChar) {
            case '/':
                while (currentChar != '\n') {
                    accept();
                }
                newline_at_end = 1;
                continue;
            case '*':
                accept();
                char prev = 0;
                while (!(currentChar == '/'
                         && prev == '*')) {
                    prev = currentChar;
                    accept();
                }
                accept();
                continue;
            case '=':
                result.kind = DIV_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = DIV;
                i++;
                break;
            }
            break;
        case '%':
            accept();
            switch (currentChar) {
            case '=':
                result.kind = MOD_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = MOD;
                i++;
                break;
            }
            break;
        case '<':
            accept();
            switch (currentChar) {
            case '<':
                accept();
                switch (currentChar) {
                case '=':
                    result.kind = LSHIFT_EQ;
                    i++;
                    accept();
                    break;
                default:
                    result.kind = LSHIFT;
                    i++;
                    break;
                }
                break;
            case '=':
                result.kind = LTE;
                i++;
                accept();
                break;
            default:
                result.kind = LT;
                i++;
                break;
            }
            break;
        case '>':
            accept();
            switch (currentChar) {
            case '>':
                accept();
                switch (currentChar) {
                case '=':
                    result.kind = RSHIFT_EQ;
                    i++;
                    accept();
                    break;
                default:
                    result.kind = RSHIFT;
                    i++;
                    break;
                }
                break;
            case '=':
                result.kind = GTE;
                i++;
                accept();
                break;
            default:
                result.kind = GT;
                i++;
                break;
            }
            break;
        case '=':
            accept();
            switch (currentChar) {
            case '=':
                result.kind = EQEQ;
                i++;
                accept();
                break;
            default:
                result.kind = EQ;
                i++;
                break;
            }
            break;
        case '^':
            accept();
            switch (currentChar) {
            case '=':
                result.kind = XOR_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = XOR;
                i++;
                break;
            }
            break;
        case '|':
            accept();
            switch (currentChar) {
            case '|':
                result.kind = LOG_OR;
                i++;
                accept();
                break;
            case '=':
                result.kind = OR_EQ;
                i++;
                accept();
                break;
            default:
                result.kind = BIT_OR;
                i++;
                break;
            }
            break;
        case '?':
            result.kind = QMARK;
            i++;
            accept();
            break;
        case ':':
            result.kind = COLON;
            i++;
            accept();
            break;
        case ';':
            result.kind = SEMICOLON;
            i++;
            accept();
            break;
        case ',':
            result.kind = COMMA;
            i++;
            accept();
            break;
        case '#':
            accept();
            switch (currentChar) {
            case '#':
                result.kind = CONCAT;
                i++;
                accept();
                break;
            default:
                result.kind = HASH;
                i++;
                break;
            }
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
            {
                j = 0;
                while ((currentChar >= 'A' && currentChar <= 'Z')
                        || (currentChar >= 'a' && currentChar <= 'z')
                        || (currentChar >= '0' && currentChar <= '9')
                        || currentChar == '_') {
                    strList_add(currentChar);
                    j++;
                    accept();
                }

                curSpelling = malloc(sizeof(char) * (j + 1));
                strList_copy(curSpelling);
                strList_free();

                int temp = isKeyword(curSpelling, j + 1);
                if (temp) {
                    result.kind = temp;
                    free(curSpelling);
                    curSpelling = NULL;
                } else {
                    result.kind = IDENTIFIER;
                }
                i++;
            }
            break;
        case '0':
            accept();
            if (currentChar == 'x'
                || currentChar == 'X') {
                if (lexHexadecimal() == HEX_FLOAT) {
                    result.kind = HEX_FLOAT;
                } else {
                    result.kind = HEX_INT;
                }
            } else {
                if (lexDecimal() == FLOAT) {
                    result.kind = FLOAT;
                } else {
                    result.kind = OCT_INT;
                }
            }
            i++;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            result.kind = lexDecimal();
            i++;
            break;
        case '\n':
            newline_at_end = 1;
        case ' ':
        case '\t':
            accept();
            continue;
        case '"':
            {
                int k = 0;
                accept();
                while (currentChar != '"') {
                    switch (currentChar) {
                    case '\\':
                        accept();
                        switch (currentChar) {
                        case '\'':
                            strList_add('\'');
                        case '"':
                            strList_add('\"');
                        case '?':
                            strList_add('\?');
                        case '\\':
                            strList_add('\\');
                        case 'a':
                            strList_add('\a');
                        case 'b':
                            strList_add('\b');
                        case 'f':
                            strList_add('\f');
                        case 'n':
                            strList_add('\n');
                        case 'r':
                            strList_add('\r');
                        case 't':
                            strList_add('\t');
                        case 'v':
                            strList_add('\v');
                            k++;
                            accept();
                            break;
                        default:
                            result.kind = ERROR;
                            i++;
                            break;
                        }
                        break;
                    default:
                        strList_add(currentChar);
                        k++;
                        accept();
                        break;
                    }
                }
                accept();
                curSpelling = malloc(sizeof(char) * (k + 1));
                strList_copy(curSpelling);
                strList_free();
                result.kind = STRING_LITERAL;
                i++;
            }
            break;
        default:
            printf("Missed: %c\n", currentChar);
            return invalid;
        }
        result.pos.startLine = curPos.startLine;
        result.pos.endLine = curPos.endLine;
        result.pos.lineStartPos = curPos.lineStartPos;
        result.pos.lineEndPos = curPos.lineStartPos;
        if (!curSpelling) {
            result.spelling.spelling = operators[result.kind];
        } else {
            result.spelling.spelling = malloc(sizeof(char) * strlen(curSpelling));
            strcpy(result.spelling.spelling, curSpelling);
        }
        return result;
    }
    result.kind = END;

    fclose(f);

    if (newline_at_end) {
        return result;
    } else {
        return invalid;
    }
}

char getNextToken(void) {
    return fgetc(f);
}

char accept(void) {
    currentChar = getNextToken();
    return currentChar;
}

size_t fsize(const char *fname) {
    struct stat st;

    if (stat(fname, &st) == 0)
        return st.st_size;

    return -1;
}

int isKeyword(const char *name, int len) {
    if (!strncmp(name, "auto", len))
        return AUTO;

    if (!strncmp(name, "break", len))
        return BREAK;

    if (!strncmp(name, "case", len))
        return CASE;

    if (!strncmp(name, "char", len))
        return CHAR;

    if (!strncmp(name, "const", len))
        return CONST;

    if (!strncmp(name, "continue", len))
        return CONTINUE;

    if (!strncmp(name, "default", len))
        return DEFAULT;

    if (!strncmp(name, "do", len))
        return DO;

    if (!strncmp(name, "double", len))
        return DOUBLE;

    if (!strncmp(name, "else", len))
        return ELSE;

    if (!strncmp(name, "enum", len))
        return ENUM;

    if (!strncmp(name, "extern", len))
        return EXTERN;

    if (!strncmp(name, "float", len))
        return FLOAT;

    if (!strncmp(name, "for", len))
        return FOR;

    if (!strncmp(name, "goto", len))
        return GOTO;

    if (!strncmp(name, "if", len))
        return IF;

    if (!strncmp(name, "inline", len))
        return INLINE;

    if (!strncmp(name, "int", len))
        return INT;

    if (!strncmp(name, "long", len))
        return LONG;

    if (!strncmp(name, "register", len))
        return REGISTER;

    if (!strncmp(name, "restrict", len))
        return RESTRICT;

    if (!strncmp(name, "return", len))
        return RETURN;

    if (!strncmp(name, "short", len))
        return SHORT;

    if (!strncmp(name, "signed", len))
        return SIGNED;

    if (!strncmp(name, "sizeof", len))
        return SIZEOF;

    if (!strncmp(name, "static", len))
        return STATIC;

    if (!strncmp(name, "struct", len))
        return STRUCT;

    if (!strncmp(name, "switch", len))
        return SWITCH;

    if (!strncmp(name, "typedef", len))
        return TYPEDEF;

    if (!strncmp(name, "union", len))
        return UNION;

    if (!strncmp(name, "unsigned", len))
        return UNSIGNED;

    if (!strncmp(name, "void", len))
        return VOID;

    if (!strncmp(name, "volatile", len))
        return VOLATILE;

    if (!strncmp(name, "while", len))
        return WHILE;

    if (!strncmp(name, "_Alignas", len))
        return _ALIGNAS;

    if (!strncmp(name, "_Alignof", len))
        return _ALIGNOF;

    if (!strncmp(name, "_Atomic", len))
        return _ATOMIC;

    if (!strncmp(name, "_Bool", len))
        return _BOOL;

    if (!strncmp(name, "_Complex", len))
        return _COMPLEX;

    if (!strncmp(name, "_Generic", len))
        return _GENERIC;

    if (!strncmp(name, "_Imaginary", len))
        return _IMAGINARY;

    if (!strncmp(name, "_Noreturn", len))
        return _NORETURN;

    if (!strncmp(name, "_Static_assert", len))
        return _STATIC_ASSERT;

    if (!strncmp(name, "_Thread_local" , len))
        return _THREAD_LOCAL;
    
    return 0;
}

int lexDecimal(void) {
    int isFloat = INTEGER, k = 0;;
    while (currentChar >= '0'
           && currentChar <= '9') {
        strList_add(currentChar);
        k++;
        accept();
    }

    if (currentChar == '.') {
        isFloat = FLOAT;
        strList_add(currentChar);
        k++;
        accept();

        while (currentChar >= '0'
               && currentChar <= '9') {
            strList_add(currentChar);
            k++;
            accept();
        }
    }

    if (currentChar == 'e'
        || currentChar == 'E') {
        isFloat = FLOAT;
        strList_add(currentChar);
        k++;
        accept();

        if (currentChar == '-' 
            || currentChar == '+') {
            strList_add(currentChar);
            k++;
            accept();
        }

        while (currentChar >= '0'
               && currentChar <= '9') {
            strList_add(currentChar);
            k++;
            accept();
        }
    }

    curSpelling = malloc(sizeof(char) * (k + 1));
    strList_copy(curSpelling);
    strList_free();

    return isFloat;
}

int lexHexadecimal(void) {
    int isFloat = HEX_INT, k = 0;;
    while ((currentChar >= '0'
            && currentChar <= '9')
           || (currentChar >= 'a'
               && currentChar <= 'f')
           || (currentChar >= 'A'
               && currentChar <= 'F')) {
        strList_add(currentChar);
        k++;
        accept();
    }

    if (currentChar == '.') {
        isFloat = HEX_FLOAT;
        strList_add(currentChar);
        k++;
        accept();

        while ((currentChar >= '0'
                && currentChar <= '9')
               || (currentChar >= 'a'
                   && currentChar <= 'f')
               || (currentChar >= 'A'
                   && currentChar <= 'F')) {
            strList_add(currentChar);
            k++;
            accept();
        }
    }

    if (currentChar == 'p'
        || currentChar == 'P') {
        isFloat = HEX_FLOAT;
        strList_add(currentChar);
        k++;
        accept();

        if (currentChar == '-'
            || currentChar == '+') {
            strList_add(currentChar);
            k++;
            accept();
        }

        while (currentChar >= '0'
               && currentChar <= '9') {
            strList_add(currentChar);
            k++;
            accept();
        }
    }

    curSpelling = malloc(sizeof(char) * (k + 1));
    strList_copy(curSpelling);
    strList_free();

    return isFloat;
}

void strList_add(char c) {
    if (str_start == NULL) {
        str_start = malloc(sizeof(StrNode));
        str_end = str_start;
        str_start->next = NULL;
        str_start->c = c;
        return;
    } else {
        str_end->next = malloc(sizeof(StrNode));
        str_end = str_end->next;
        str_end->c = c;
        str_end->next = NULL;
    }
}

void strList_copy(char *c) {
    StrNode *temp = str_start;
    int i = 0;

    while (temp != NULL) {
        c[i] = temp->c;;
        temp = temp->next;
        i++;
    }
    c[i] = '\0';
}

void strList_free(void) {
    StrNode *temp = str_start;
    while (temp != NULL) {
        temp = temp->next;
        free(str_start);
        str_start = temp;
    }

    str_start = NULL;
    str_end = NULL;
}
