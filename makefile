Emulator: Emulator.c CPU.c
	gcc -o Emulator Emulator.c CPU.c -I.
	
CPU: CPU.c
	gcc -o CPU CPU.c -I.
	
clean:
	rm *.o *.exe