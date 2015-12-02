#include <stdio.h>
#include <stdlib.h>
#include "Emulator.h"
#include "string.h"
#include "math.h"

//along the 5 rows are the commands being executed by each stage in the pipeline
//the commands are stored as chars in the columns
char pipelineCommands[5][27] ;
FILE *fp;
enum mnemonics{
	MOV = 11111111,
	JMP = 11111110,
	CMP = 11111101,
	INC = 11111100,
};

int incnum(int num)
{
	int initnum = num;
	float res = 0;
	float buf = 0;
	int i;
	for (i = 0; i < 8 ; i++) {
		int dig = initnum%10;
		if(dig == 1) {
			buf = buf + pow(10,i);
		} else {
			res = num - buf;
			res = res + pow(10,i);
			break;
		}
		initnum = initnum/10;
	}
	return (int)res;
	
}

void inc(char num[9])
{
	int i;
	for (i = 7; i > 0 ; i-- ) {
		if(strncmp(&num[i],"1",1) == 0) {
			num[i] = 48;
		} else {
			num[i] = 49;
			break;
		}
	}
	
}

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
		printf("Success, Instructions Loaded\n\n");
	}
}

//shifts right everything in the commands array, leaving the first element null
void shiftPipeline()
{
	strncpy(pipelineCommands[4],pipelineCommands[3],27);
	strncpy(pipelineCommands[3],pipelineCommands[2],27);
	strncpy(pipelineCommands[2],pipelineCommands[1],27);
	strncpy(pipelineCommands[1],pipelineCommands[0],27);
}

void fetch()
{
	if(fscanf(fp, "%[^\n]\n", pipelineCommands[0]) == EOF) {
		//printf("EOF!\n");
		strncpy(pipelineCommands[0],"00000000",26);
	} else {
		printf("Fetch: %s\n",pipelineCommands[0]);
	}
	setInstrLine(strlen(pipelineCommands[0]) + getInstrLine());
}

void decode()
{
	
	//substringing the mnemonic out of the line	
	char buf[8];
	strncpy(buf,pipelineCommands[1],8);
	buf[8] = (char)0;
	
	switch(atoi(buf)) {
		case MOV :
			printf("Decode: %s -> MOV\n",pipelineCommands[1]);
			break;
		case JMP :
			printf("Decode: %s -> JMP\n",pipelineCommands[1]);
			break;
		case CMP : 
			printf("Decode: %s -> CMP\n",pipelineCommands[1]);
			break;
		case INC :
			printf("Decode: %s -> INC\n",pipelineCommands[1]);	
			break;
	}
}	

void execute()
{
	char buf[8];
	strncpy(buf,pipelineCommands[2],8);
	buf[8] = (char)0;	
	char op1buf[9];
	strncpy(op1buf,pipelineCommands[2]+9,8);
	buf[9] = (char)0;
	char op2buf[9];
	strncpy(op2buf,pipelineCommands[2]+18,8);
	buf[9] = (char)0;
	
	switch(atoi(buf)) {
		case MOV :
			printf("Execute: mov statement, nothing to execute!\n");
			break;
		case JMP :
			printf("Execute: updating PSW, loading decode with NOP to flush out the pipeline\n");
			break;
		case CMP :
			;char value1buf[9];
			char value2buf[9];
			getValAtAddr(atoi(op1buf),value1buf);
			getValAtAddr(atoi(op2buf),value2buf);
			if(strncmp(value1buf,value2buf,8) == 0) {
				printf("Execute: Compare returned true, setting ZF to 1\n");
			} else {
				printf("Execute: Compare returned false, setting ZF to 0\n");
			}
			break;
		case INC : 
			;char valuebuf[9];
			getValAtAddr(atoi(op1buf),valuebuf);
			inc(valuebuf);
			printf("Execute: XOR result is %s\n",valuebuf);
			break;
	}
}


//simulates one clock cyle
void executeCycle()
{
	/**char test[8] = "00000001";
	printf("orig %s\n",test);
	inc(test);
	printf("final %s\n",test);**/
	/**int test = 1;
	printf("init %i\n",test);
	int res2 = incnum(test);
	printf("final %i\n",res2);**/
	//printf("1:%s\n2:%s\n3:%s\n",pipelineCommands[0],pipelineCommands[1],pipelineCommands[2]);
	fetch();
	decode();
	execute();
	shiftPipeline();
}



