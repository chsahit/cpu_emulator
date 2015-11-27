Emulator: Emulator.c CPU.c
	gcc -o Emulator Emulator.c CPU.o -I.
	
CPU: CPU.c
	gcc -c CPU.c -I. 
	
clean:
	rm *.o *.exe