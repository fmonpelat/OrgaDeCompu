# Makefile

CC=gcc
DEBUG=gdb
CFLAGS= -Wall -g -O3 -mabicalls
OUTFILE=binaries/tp0
INPUT=input/input.txt
OUTPUT=input/out.txt
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
	$(info Test cases...)
	$(info 1. Printing help)
#tp0 -h 
	$(info 2. Test normal execution)
#tp0 -i inputs/input.txt -o output/output.txt
	$(info 3. Test decode)
#tp0 -d -i input/qb64 -o output/output.txt
	$(info 4. Test empty input)
#tp0 -i inputs/empty.txt -o output/output.txt
	$(info 5. Test long input)
#tp0 -i inputs/long.txt -o output/output.txt
	$(info 6. Test multiple of three input)
#tp0 -i inputs/multiplode3.txt -o output/output.txt
	$(info 7. Test pipe input, stdout output)
#cat input.txt | ./tp0
	$(info 8. Test redirect symbols input output)
#tp0 < input.txt > output.txt
	$(info 9. Test redirect symbols only input)
#tp0 -o output.txt < input.txt
	$(info 10. Test redirect symbols only output)
#tp0 -i input.txt > output.txt

# Para chequear las salidas y ver si todo esta bien podemos usar esto
# if [ $? -eq 0 ]; then echo OK; else echo FAIL; fi;

clean:
	rm -rf binaries/* test/* input/out*
