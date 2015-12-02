#include <stdio.h>
#include "Emulator.h"

int main()
{
	initCPU("testfile.sasm.o");
	int i;
	char val[9] = "10000000";
	char buf[9];
	setValAtAddr(1,val);
	getValAtAddr(1,buf);
	printf("buf has %s\n",buf);
	//for ( i = 0; i < 6; i = i + 1 ) {
	//	executeCycle();
	//	printf("\n");
	//}
	return 0;
}