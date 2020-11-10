# Makefile

CC=gcc
DEBUG=gdb
CFLAGS= -Wall -g -O3 -mabicalls -Wconversion -Wtype-limits -pedantic
OUTFILE=binaries/tp1
OUTPUT=input/output.txt
OBJ= binaries/tp1
SOURCES=src/main.c src/euclidean.c asm/asmEuclidean.S
EXECUTABLE=main

all: $(EXECUTABLE)

main: 
	@mkdir -p binaries
	$(CC) $(CFLAGS) -o $(OBJ) $(SOURCES)

debug: all
	$(DEBUG) -ex 'tui enable' -ex 'break main' --args $(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'
	stty sane

debugasm: all
	$(DEBUG) -ex 'tui enable' -ex 'break asmMcd' -ex 'break asmMcm' -ex 'layout asm' -ex 'layout regs' --args $(OUTFILE) -i '$(INPUT)' -o '$(OUTPUT)'
	stty sane

test: all
	tests/tests.sh $(OUTFILE)

clean:
	rm -rf binaries/* inputs/*out.txt
