#include <stdio.h>
#include "Emulator.h"

int main()
{
	initCPU("testfile.sasm.o");
	initMem();
	int i;
	for ( i = 0; i < 15; i = i + 1 ) {
		executeCycle();
		printf("\n");
	}
	return 0;
}