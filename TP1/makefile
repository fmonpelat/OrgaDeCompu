# Makefile

CC=gcc
DEBUG=gdb
CFLAGS= -Wall -g -O3 -mabicalls
OUTFILE=binaries/tp0
INPUT=inputs/input.txt
OUTPUT=inputs/out.txt
OBJ= binaries/tp0
SOURCES=src/main.c 
EXECUTABLE=main


all: $(EXECUTABLE)

main:
	$(CC) $(CFLAGS) -o $(OBJ) $(SOURCES)

debug: all
	$(DEBUG) -ex 'tui enable' -ex 'break main' --args $(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'
	stty sane
debugasm: all
	-$(DEBUG) -ex 'tui enable' -ex 'break mergesort' -ex 'merge' -ex 'layout asm' -ex 'layout regs' --args $(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'
	stty sane
test: all
	$(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'

clean:
	rm -rf binaries/*
