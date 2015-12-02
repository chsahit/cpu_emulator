#include <stdio.h>
#include "Emulator.h"


int instrLine = 0;
int memoryLookup[256][8];

int getInstrLine()
{
	return instrLine;
}

void setInstrLine(int line)
{
	instrLine = line;
}

void getValAtAddr(int addr, char buf[9])
{
	strncpy(buf,memoryLookup[addr],8);
	buf[8] = (char)0;
	return;
}

void setValAtAddr(int addr, char buf[9])
{
	strncpy(memoryLookup[addr],buf,8);
}

int binToDec(int binNum)
{
	return 0;
}

