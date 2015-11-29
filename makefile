all: Emulator.c CPU.c Memory.c
	gcc -o Emulator Emulator.c CPU.c Memory.c -I.
	
Emulator: Emulator.c CPU.o Memory.o
	gcc -o Emulator Emulator.c CPU.o Memory.o -I.
	
Memory: Memory.c
	gcc -c Memory.c -I.
	
CPU: CPU.c
	gcc -c CPU.c -I. 
	
clean:
	rm *.o *.exe