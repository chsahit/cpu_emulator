#include <stdio.h>
#include <stdlib.h>
#include "Emulator.h"
#include "string.h"

//along the 5 rows are the commands being executed by each stage in the pipeline
//the commands are stored as chars in the columns
char pipelineCommands[5][26] ;
FILE *fp;
enum mnemonics{
	MOV = 11111111,
	JMP = 11111110,
	CMP = 11111101,
	INC = 11111100,
};


void initCPU(char filename[])
{
	//pipelineCommands = (char) malloc(5 * sizeof(char));
	//for(int i = 0; i < 6; i++) {
	//	pipelineCommands[i] = malloc(26 * sizeof(char));
	//}
	printf("CPU Loaded\n");
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("File IO Error\n");
	} else {
		printf("Success, Instructions Loaded\n");
	}
}

//shifts right everything in the commands array, leaving the first element null
void shiftPipeline()
{
	strncpy(pipelineCommands[4],pipelineCommands[3],8);
	strncpy(pipelineCommands[3],pipelineCommands[2],8);
	strncpy(pipelineCommands[2],pipelineCommands[1],8);
	strncpy(pipelineCommands[1],pipelineCommands[0],8);
}

void fetch()
{
	//printf("Instruction Line: ");
	//printf("%i\n",getInstrLine());
	
	fscanf(fp, "%[^\n]\n", pipelineCommands[0]);
	printf("Fetch: %s\n",pipelineCommands[0]);
	setInstrLine(strlen(pipelineCommands[0]) + getInstrLine());
	//printf("%i",strlen(pipelineCommands[1]));
	
}

void decode()
{
	//substringing the mnemonic out of the line	
	char buf[8];
	strncpy(buf,pipelineCommands[1],8);

	switch(atoi(buf)) {
		case MOV :
			printf("Decode: %s -> MOV\n",pipelineCommands[1]);
			break;
		case JMP :
			printf("Decoded %s -> JMP\n",pipelineCommands[1]);
			break;
		case CMP : 
			printf("Decoded %s -> CMP\n",pipelineCommands[1]);
			break;
		case INC :
			printf("Decoded %s -> INC\n	",pipelineCommands[1]);	
			break;
	}
}

//simulates one clock cyle
void executeCycle()
{
	shiftPipeline();
	fetch();
	decode();
}

