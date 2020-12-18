# Makefile

CC=gcc
DEBUG=gdb
CFLAGS= -Wall -g -O3 -Wconversion -Wtype-limits -pedantic
OUTFILE=binaries/tp1
OUTPUT=input/output.txt
OBJ= binaries/tp1
MAIN=src/main.c
C_SOURCES=src/euclidean.c
ASM_SOURCES=asm/asmEuclidean.S
EXECUTABLE=main

ifeq ($(findstring mips,$(shell uname -m)),mips)
	SOURCES = -mabicalls -DMIPS -o $(OBJ) $(MAIN) $(ASM_SOURCES)
else
	SOURCES = -o $(OBJ) $(MAIN) $(C_SOURCES)
endif

all: $(EXECUTABLE)

main: 
	@mkdir -p binaries
	$(CC) $(CFLAGS) $(SOURCES)

debug: all
	$(DEBUG) -ex 'tui enable' -ex 'break main' --args $(OUTFILE) 20 30
	stty sane

debugasm: all
	$(DEBUG) -ex 'tui enable' -ex 'break asmMcd' -ex 'break asmMcm' -ex 'layout asm' -ex 'layout regs' --args $(OUTFILE) 20 30
	stty sane

test: all
	tests/tests.sh $(OUTFILE)

clean:
	rm -rf binaries/* inputs/*out.txt
