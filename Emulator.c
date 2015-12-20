#include <stdio.h>
#include "Emulator.h"

int main()
{
	initCPU("testfile.sasm.o");
	initMem();
	printf("\n");
	int i;
	for ( i = 0; i < 16; i = i + 1 ) {
		executeCycle();
		printf("\n");
	}
	return 0;
}