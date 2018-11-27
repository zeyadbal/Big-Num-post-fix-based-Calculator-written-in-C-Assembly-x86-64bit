all: calc

calc: main.o asm.o

	gcc -Wall -g -o calc main.o asm.o
	
main.o: main.c
	gcc -Wall -g -c -o main.o main.c

asm.o: asm.s
	nasm -g -f elf64 -w+all -o asm.o asm.s

clean:
	rm -f *.o calc
