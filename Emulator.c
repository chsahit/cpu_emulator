#include <stdio.h>
#include "Emulator.h"

int main()
{
	initCPU("testfile.sasm.o");
	initMem();
	int i;
	for ( i = 0; i < 21; i = i + 1 ) {
		executeCycle();
		printf("\n");
	}
	return 0;
}