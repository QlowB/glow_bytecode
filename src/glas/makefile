CC          := gcc
LEX         := flex
YACC        := bison
CFLAGS      := -Wall -std=gnu99
LDFLAGS     :=
DEPS        := *.h
OBJECTS     := bytecode_assembler.o instruction_compiler.o conversion_table.o main.c parser.o tokens.o run_options.o ../glob_file.o ../glow_error.o
EXECUTABLE  := glas

all: $(OBJECTS) $(EXECUTABLE)

tokens.c: parser.c
	$(LEX) -t tokens.l > tokens.c

parser.c:
	$(YACC) -d -o parser.c parser.y

tokens.o: tokens.c
	$(CC) -w -c -o $@ tokens.c



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


.PHONY: clean
clean:
	rm -f *.o glas
	rm -f parser.c parser.h tokens.c
