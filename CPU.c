#include <stdio.h>
#include "Emulator.h"

//along the 5 rows are the commands being executed by each stage in the pipeline
//the commands are stored as chars in the columns
char pipelineCommands[5][24];
FILE *fp;

void initCPU(char filename[])
{
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("File IO Error");
	} else {
		printf("Success, Instructions Loaded");
	}
}

//shifts right everything in the commands array, leaving the first element null
void shiftPipeline()
{
	
}

void fetch()
{
	fgets(pipelineCommands[0],24,(FILE*) fp);
}

//simulates one clock cyle
void executeCycle()
{
	shiftPipeline();
}


void printWorking()
{
	printf("CPU Loaded\n");
	return;
}
