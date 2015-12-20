#include <stdio.h>
#include "Emulator.h"
#include "string.h"
#include "math.h"

/**Emulates a hard disk w/ 256 bytes of memory**/

int instrLine = 0;
char memoryLookup[256][8];
char lockTable[256][1];

int isLocked(int addr)
{
	if(addr < 0 || addr > 257) {
		return 0;
	}
	return lockTable[binToDec(addr)][0];
}

void setLocked(int addr,int locked)
{
	lockTable[binToDec(addr)][0] = (char) 49;
}

void initMem()
{
	memset(memoryLookup,48,sizeof memoryLookup);
	memset(lockTable,48,sizeof memoryLookup);
	printf("Memory Initialized\n");
}
int getInstrLine()
{
	return instrLine;
}

void setInstrLine(int line)
{
	instrLine = line;
}

int binToDec(int binNum)
{
	int decimal=0, i=0, rem;
    while (binNum!=0)
    {
        rem = binNum%10;
        binNum/=10;
        decimal += rem*pow(2,i);
        ++i;
    }
    return decimal;
}

void getValAtAddr(int addr, char buf[9])
{
	strncpy(buf,memoryLookup[binToDec(addr)],8);
	buf[8] = (char)0;
	return;
}

void setValAtAddr(int addr, char buf[9])
{
	strncpy(memoryLookup[binToDec(addr)],buf,8);
}



