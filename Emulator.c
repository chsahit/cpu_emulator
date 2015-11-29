#include <stdio.h>
#include "Emulator.h"

int main()
{
	initCPU("testfile.sasm.o");
	executeCycle();
	return 0;
}