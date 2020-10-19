# Makefile

CC=gcc
DEBUG=gdb
CFLAGS= -Wall -g -O3 -mabicalls -Wconversion -Wtype-limits -pedantic
OUTFILE=binaries/tp0
INPUT=input/input.txt
OUTPUT=input/output.txt
OBJ= binaries/tp0
SOURCES=src/main.c src/encriptado_base64.c
EXECUTABLE=main

all: $(EXECUTABLE)

main:
	$(CC) $(CFLAGS) -o $(OBJ) $(SOURCES)

debug: all
	$(DEBUG) -ex 'tui enable' -ex 'break main' --args $(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'
	stty sane

debugasm: all
	$(DEBUG) -ex 'tui enable' -ex 'break mergesort' -ex 'merge' -ex 'layout asm' -ex 'layout regs' --args $(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'
	stty sane

test: all
	tests/tests.sh $(OUTFILE)

clean:
	rm -rf binaries/* inputs/*out.txt
