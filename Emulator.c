#include <stdio.h>
#include "Emulator.h"

int main()
{
	initCPU("testfile.sasm.o");
	executeCycle();
	printf("\n");
	executeCycle();
	return 0;
}