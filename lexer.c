/*
 * Will Korteland
 * 21st August 2014
 * Parsing module for compiler
 */

/*
 * TODO:
 * source positions
 * literals
 * whitespace
 */

#include "lexer.h"

char accept(void);
char nextToken(void);
size_t fsize(const char *fname);
int isKeyword(const char *name, int len);

static FILE *f;
static char currentChar;

int parse(const char *fname) {
    f = fopen(fname, "r");
    if (!f) {
        printf("Error: couldn't open source file\n");
        return -1;
    }

    size_t len = fsize(fname), i = 0, j = 0, strMax = 0;;
    int result[len];
    accept();
    while (currentChar != EOF) {
        switch (currentChar) {
        case '[':
            result[i] = LBRACKET;
            i++;
            accept();
            break;
        case ']':
            result[i] = RBRACKET;
            i++;
            accept();
            break;
        case '(':
            result[i] = LPAREN;
            i++;
            accept();
            break;
        case ')':
            result[i] = RPAREN;
            i++;
            accept();
            break;
        case '{':
            result[i] = LCURLY;
            i++;
            accept();
            break;
        case '}':
            result[i] = RCURLY;
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
                    result[i] = ELLIPSE;
                    i++;
                    accept();
                    break;
                default:
                    result[i] = ERROR;
                    i++;
                    break;
                }
            default:
                result[i] = DOT;
                i++;
                break;
            }
            break;
        case '-':
            accept();
            switch (currentChar) {
            case '>':
                result[i] = S_DEREF;
                i++;
                accept();
                break;
            case '-':
                result[i] = DECREMENT;
                i++;
                accept();
                break;
            case '=':
                result[i] = MINUS_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = MINUS;
                i++;
                break;
            }
            break;
        case '+':
            accept();
            switch (currentChar) {
            case '+':
                result[i] = INCREMENT;
                i++;
                accept();
                break;
            case '=':
                result[i] = PLUS_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = PLUS;
                i++;
                break;
            }
            break;
        case '&':
            accept();
            switch (currentChar) {
            case '&':
                result[i] = LOG_AND;
                i++;
                accept();
                break;
            case '=':
                result[i] = AND_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = BIT_AND;
                i++;
                break;
            }
            break;
        case '*':
            accept();
            switch (currentChar) {
            case '=':
                result[i] = MULT_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = STAR;
                i++;
                break;
            }
            break;
        case '~':
            result[i] = TILDE;
            i++;
            accept();
            break;
        case '!':
            accept();
            switch (currentChar) {
            case '=':
                result[i] = NEQ;
                i++;
                accept();
                break;
            default:
                result[i] = NOT;
                i++;
                break;
            }
            break;
        case '/':
            accept();
            switch (currentChar) {
            case '/':
                //printf("TODO: whitespace\n");
                accept();
                break;
            case '*':
                //printf("TODO: whitespace\n");
                accept();
                break;
            case '=':
                result[i] = DIV_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = DIV;
                i++;
                break;
            }
            break;
        case '%':
            accept();
            switch (currentChar) {
            case '=':
                result[i] = MOD_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = MOD;
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
                    result[i] = LSHIFT_EQ;
                    i++;
                    accept();
                    break;
                default:
                    result[i] = LSHIFT;
                    i++;
                    break;
                }
                break;
            case '=':
                result[i] = LTE;
                i++;
                accept();
                break;
            default:
                result[i] = LT;
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
                    result[i] = RSHIFT_EQ;
                    i++;
                    accept();
                    break;
                default:
                    result[i] = RSHIFT;
                    i++;
                    break;
                }
                break;
            case '=':
                result[i] = GTE;
                i++;
                accept();
                break;
            default:
                result[i] = GT;
                i++;
                break;
            }
            break;
        case '=':
            accept();
            switch (currentChar) {
            case '=':
                result[i] = EQEQ;
                i++;
                accept();
                break;
            default:
                result[i] = EQ;
                i++;
                break;
            }
            break;
        case '^':
            accept();
            switch (currentChar) {
            case '=':
                result[i] = XOR_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = XOR;
                i++;
                break;
            }
            break;
        case '|':
            accept();
            switch (currentChar) {
            case '|':
                result[i] = LOG_OR;
                i++;
                accept();
                break;
            case '=':
                result[i] = OR_EQ;
                i++;
                accept();
                break;
            default:
                result[i] = BIT_OR;
                i++;
                break;
            }
            break;
        case '?':
            result [i] = QMARK;
            i++;
            accept();
            break;
        case ':':
            result[i] = COLON;
            i++;
            accept();
            break;
        case ';':
            result[i] = SEMICOLON;
            i++;
            accept();
            break;
        case ',':
            result[i] = COMMA;
            i++;
            accept();
            break;
        case '#':
            accept();
            switch (currentChar) {
            case '#':
                result[i] = CONCAT;
                i++;
                accept();
                break;
            default:
                result[i] = HASH;
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
                strMax = 60;
                char *name;
                char temp[strMax];
                name = temp;
                while ((currentChar >= 'A' && currentChar <= 'Z')
                       || (currentChar >= 'a' && currentChar <= 'z')
                       || (currentChar >= '0' && currentChar <= '9')
                       || currentChar == '_') {
                    name[j] = currentChar;
                    j++;
                    if (j >= strMax - 2) {
                        char temp2[strMax * 2];
                        strncpy(temp2, name, strMax - 1);
                        strMax *= 2;
                        name = temp2;
                    }
                    accept();
                }
                name[j] = '\0';

                if (isKeyword(name, j + 1)) {
                    result[i] = KEYWORD;
                    i++;
                } else {
                    result[i] = IDENTIFIER;
                    i++;
                }
            }
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            //printf("TODO: numbers\n");
            accept();
            break;
        case ' ':
        case '\t':
        case '\n':
            //printf("TODO: whitespace\n");
            accept();
            break;
        case '"':
            //printf("TODO: strings\n");
            accept();
            break;
        case '\\':
            //printf("TODO: escapes\n");
            accept();
            break;
        default:
            printf("Missed: %c\n", currentChar);
            return -1;
        }
    }

    fclose(f);
    for (int j = 0; j < i; j++) {
        printf("%s ", operators[result[j]]);
    }
    printf("\n");
    return 0;
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
    if (!strncmp(name, "auto", len)
        || !strncmp(name, "break", len)
        || !strncmp(name, "case", len)
        || !strncmp(name, "char", len)
        || !strncmp(name, "const", len)
        || !strncmp(name, "continue", len)
        || !strncmp(name, "default", len)
        || !strncmp(name, "do", len)
        || !strncmp(name, "double", len)
        || !strncmp(name, "else", len)
        || !strncmp(name, "enum", len)
        || !strncmp(name, "extern", len)
        || !strncmp(name, "float", len)
        || !strncmp(name, "for", len)
        || !strncmp(name, "goto", len)
        || !strncmp(name, "if", len)
        || !strncmp(name, "inline", len)
        || !strncmp(name, "int", len)
        || !strncmp(name, "long", len)
        || !strncmp(name, "register", len)
        || !strncmp(name, "restrict", len)
        || !strncmp(name, "return", len)
        || !strncmp(name, "short", len)
        || !strncmp(name, "signed", len)
        || !strncmp(name, "sizeof", len)
        || !strncmp(name, "static", len)
        || !strncmp(name, "struct", len)
        || !strncmp(name, "switch", len)
        || !strncmp(name, "typedef", len)
        || !strncmp(name, "union", len)
        || !strncmp(name, "unsigned", len)
        || !strncmp(name, "void", len)
        || !strncmp(name, "volatile", len)
        || !strncmp(name, "while", len)
        || !strncmp(name, "_Alignas", len)
        || !strncmp(name, "_Alignof", len)
        || !strncmp(name, "_Atomic", len)
        || !strncmp(name, "_Bool", len)
        || !strncmp(name, "_Complex", len)
        || !strncmp(name, "_Generic", len)
        || !strncmp(name, "_Imaginary", len)
        || !strncmp(name, "_Noreturn", len)
        || !strncmp(name, "_Static_assert", len)
        || !strncmp(name, "_Thread_local" , len)) {
        return 1;
    }
    return 0;
}
