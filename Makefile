TARGET=compiler
CC=clang
LIBS=-lm
CFLAGS=-Wall -Werror -g -std=c11
HEADERS=lexer.h

default: $(TARGET)
all: default

compiler: lexer.c $(HEADERS)
	$(CC) $(CFLAGS) $(LIBS) main.c lexer.c -o $(TARGET)

clean:
	-rm -rf *.o
	-rm -f $(TARGET)
