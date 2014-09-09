TARGET=compiler
CC=clang
LIBS=-lm
CFLAGS=-Wall -Werror -g -std=c11
HEADERS=source/c/lexer.h

default: $(TARGET)
all: default

compiler: source/c/lexer.c $(HEADERS)
	$(CC) $(CFLAGS) $(LIBS) main.c source/c/lexer.c -o $(TARGET)

clean:
	-rm -rf *.o
	-rm -f $(TARGET)
