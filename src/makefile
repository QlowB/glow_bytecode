CC            := gcc
CFLAGS        := -Wall -std=gnu99
LDFLAGS       :=
OBJECTS       := bytecode_interpreter.o main.o
ASM_SUBDIR    := glas
DISASM_SUBDIR := glds
EXECUTABLES   := glow $(ASM_SUBDIR)/glas # $(DISASM_SUBDIR)/glds
INSTALLPREF   := /usr/local

all: clean_glas $(OBJECTS) $(EXECUTABLES)


.PHONY: clean_glas
clean_glas:
	rm -f $(ASM_SUBDIR)/*.o

.PHONY: clean_glds
clean_glds:
	rm -f $(DISASM_SUBDIR)/*.o

bytecode_interpreter.o: bytecode_interpreter.c
	$(CC) -O3 -c -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

glow: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(ASM_SUBDIR)/glas:
	cd $(ASM_SUBDIR) && $(MAKE) all

$(DISASM_SUBDIR)/glds:
	cd $(DISASM_SUBDIR) && $(MAKE) all

.PHONY: install
install: all
	install $(EXECUTABLES) $(INSTALLPREF)/bin

.PHONY: clean
clean:
	rm -f *.o glow
	cd $(ASM_SUBDIR) && $(MAKE) clean
	cd $(DISASM_SUBDIR) && $(MAKE) clean

