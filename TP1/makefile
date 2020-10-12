# Makefile

CC=gcc
DEBUG=gdb
CFLAGS= -Wall -g -O3 -mabicalls
OUTFILE=binaries/tp1
INPUT=inputs/input.txt
OUTPUT=inputs/out.txt
OBJ= binaries/tp1
SOURCES=src/merge_sort_mips.c 
ASM=asm/asmMergeSort.S asm/mymalloc.S
EXECUTABLE=main


all: $(EXECUTABLE)

main:
	$(CC) $(CFLAGS) -o $(OBJ) $(SOURCES) $(ASM)

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
