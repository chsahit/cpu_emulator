#include <stdio.h>
#include <stdlib.h>
#include "Emulator.h"
#include "string.h"
#include "math.h"


//along the 5 rows are the commands being executed by each stage in the pipeline
//the commands are stored as chars in the columns
char pipelineCommands[5][27] ;
FILE *fp; //input file
int halted; //flag for if there was a pipeline hazard
char tempComm[27]; //buffer for offending instruction in case there was a p. hazard
int lnNum; //index w/location in the input file 
int ZF = 0; //ZF, written to by CMP and acted on by JMP
enum mnemonics{
	MOV = 11111111,
	JNE = 11111110,
	CMP = 11111101,
	INC = 11111100,
	L   = 11111011,
	NOP = 11111010,
};

//get one instruction from the file and write it to buf
void readLn(char buf[27])
{
	memset(buf,(char)0,sizeof buf);
	fread(buf,27,1,fp);
	buf[26] = (char)0;
	//printf(buf);
}

//add 1 to the binary number stored in num[]
//simulates num XOR 1
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

//opens file for reading and initializes flags
void initCPU(char filename[])
{
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("File IO Error\n");
	} else {
		printf("Success, Instructions Loaded\n");
	}
	halted = 0;
	lnNum = 0;
	printf("CPU Loaded\n");
}


//shifts right everything in the commands array, leaving the first element null
void shiftPipeline()
{
	strncpy(pipelineCommands[4],pipelineCommands[3],27);
	strncpy(pipelineCommands[3],pipelineCommands[2],27);
	strncpy(pipelineCommands[2],pipelineCommands[1],27);
	strncpy(pipelineCommands[1],pipelineCommands[0],27);
}

//fetch an instruction and check if it is safe to run
//if it is, load it into the pipeline
void fetch()
{
	if(halted == 0) { 
		readLn(pipelineCommands[0]);
		fseek(fp,27,lnNum);
		lnNum = lnNum + 27;
		if(strncmp(pipelineCommands[0],"",1) == 0) {
			printf("Fetch:null\n");
			strncpy(pipelineCommands[0],"11111010 11111111 11111111",27);
		}
		if(EOF != EOF)  {
			printf("EOF!\n");
			//terminated++;
		} else {
			char mnemonicbuf[9];
			strncpy(mnemonicbuf,pipelineCommands[0],8);
			char op1buf[9];
			strncpy(op1buf,pipelineCommands[0]+9,8);
			char op2buf[9];
			op1buf[8] = (char) 0;
			strncpy(op2buf,pipelineCommands[0]+18,8);
			op2buf[8] = (char) 0;
			if(isLocked(atoi(op1buf)) == 49 || (isLocked(atoi(op2buf)) == 49 && strncmp(op2buf,"",1) != 0)) {
				printf("Fetch: pipeline Hazard Detected!, Loading pipeline w/NOP\n");
				printf("Fetch: stashing %s\n",pipelineCommands[0]);
				strncpy(tempComm,pipelineCommands[0],27);
				strncpy(pipelineCommands[0],"11111010 00000000",17); 
				halted = 1;
			}
			printf("Fetch: %s\n",pipelineCommands[0]);
		}
	} else if (halted == 1) {
		strncpy(pipelineCommands[0],"11111010 00000000",17);
		halted = 2;
		printf("Fetch: Loading second NOP into pipeline\n");
	} else if (halted == 2) {
		printf("Pipeline hazard cleared, loading %s into pipeline\n",tempComm);
		strncpy(pipelineCommands[0],tempComm,27);
		halted = 0;
	}
}

void decode()
{
	
	//substringing the mnemonic out of the line	
	char buf[8];
	strncpy(buf,pipelineCommands[1],8);
	buf[8] = (char)0;
	char op1buf[9];
	strncpy(op1buf,pipelineCommands[1]+9,8);
	op1buf[8] = (char)0;
	
	switch(atoi(buf)) {
		case MOV :
			printf("Decode: %s -> MOV\n",pipelineCommands[1]);
			break;
		case JNE :
			printf("Decode: %s -> JNE\n",pipelineCommands[1]);
			break;
		case CMP : 
			printf("Decode: %s -> CMP\n",pipelineCommands[1]);
			break;
		case INC :
			printf("Decode: %s -> INC\n",pipelineCommands[1]);	
			break;
		case L : 
			printf("Decode: %s -> L\n",pipelineCommands[1]);
			setLocked(atoi(op1buf),49);
			break;
		case NOP : 
			printf("Decode: %s -> NOP\n",pipelineCommands[1]);
	}
}	

//execute stage of the pipeline. does things the ALU would do, i.e math
void execute()
{
	char buf[8];
	strncpy(buf,pipelineCommands[2],8);
	buf[8] = (char)0;	
	char op1buf[9];
	strncpy(op1buf,pipelineCommands[2]+9,8);
	op1buf[8] = (char)0;
	char op2buf[9];
	strncpy(op2buf,pipelineCommands[2]+18,8);
	op2buf[8] = (char)0;
	
	switch(atoi(buf)) {
		case MOV :
			printf("Execute: mov statement, nothing to execute! Locking %s\n",op1buf);
			break;
		case JNE :
			if(ZF == 0) { //look for the val of the ZF
				lnNum = 27 * atoi(op1buf) - 27; //compute the new character location
				printf("Execute: updating index to %i\n",lnNum);
			} else {
				printf("Execute: Not Jumping because ZF is 1\n");
			}
			break;
		case CMP :
			;char value1buf[9];
			char value2buf[9]; 
			getValAtAddr(atoi(op1buf),value1buf);
			getValAtAddr(atoi(op2buf),value2buf);
			if(strncmp(value1buf,value2buf,8) == 0) {
				printf("Execute: Compare returned true, setting ZF to 1\n");
				ZF = 1;
			} else {
				printf("Execute: Compare returned false, setting ZF to 0\n");
				ZF = 0;
			}
			break;
		case INC : 
			;char valuebuf[10];
			getValAtAddr(atoi(op1buf),valuebuf);
			inc(valuebuf);
			valuebuf[9] = (char)0;
			printf("Execute: XOR result is %s\n",valuebuf);
			break;
		case L:
			printf("Execute: Load is not completed until WB Locking %s\n",op1buf);
	}
}

//does memory reads if necessary
void memAccess()
{
	char buf[8];
	strncpy(buf,pipelineCommands[3],8);
	buf[8] = (char)0;	
	char op1buf[9];
	strncpy(op1buf,pipelineCommands[3]+9,8);
	buf[8] = (char)0;
	char op2buf[9];
	strncpy(op2buf,pipelineCommands[3]+18,8);
	buf[8] = (char)0;
	switch(atoi(buf)) {
		char val1[9]; 
		case MOV:
			//going from reg to mem
			if(atoi(op1buf) > 11 && atoi(op2buf) < 11) { 
				getValAtAddr(atoi(op2buf),val1);
				printf("Memory Access: Wrote %s to %s\n",val1,op1buf);
				 //going from mem to reg
			} else if (atoi(op1buf) < 11 && atoi(op2buf) > 11) {
				getValAtAddr(atoi(op2buf), val1);
				setValAtAddr(atoi(op1buf),val1);
				printf("Memory Access: Read value %s\n",val1);
			}
			break;
	}
}

void writeBack()
{
	char buf[8];
	strncpy(buf,pipelineCommands[4],8);
	buf[8] = (char)0;	
	char op1buf[9];
	strncpy(op1buf,pipelineCommands[4]+9,8);
	op1buf[8] = (char)0;
	char op2buf[9];
	strncpy(op2buf,pipelineCommands[4]+18,8);
	op2buf[8] = (char)0;
	switch(atoi(buf)) {
		case MOV:
			//register to register
			if(atoi(op1buf) < 11 && atoi(op2buf) < 11) {
				char val1[9]; char val2[9];
				getValAtAddr(atoi(op2buf),val1);
				setValAtAddr(atoi(op1buf),val1);
				printf("Register Write Back: Wrote %s to %s\n",val1,op1buf);
				break;
			}	
		
		case INC:
			;char valuebuf[9];
			getValAtAddr(atoi(op1buf),valuebuf);
			inc(valuebuf);
			setValAtAddr(atoi(op1buf),valuebuf);
			printf("Register Write Back: Wrote %s to %s\n",valuebuf,op1buf);
			break;
			
		case L:
			//printf("Register We: Writing %s to %s\n",op2buf,op1buf);
			;char val1[9];
			setValAtAddr(atoi(op1buf),op2buf);
			char val2[10];
			getValAtAddr(atoi(op1buf),val2);
			val2[9] = (char)0;
			printf("Register Write Back: Wrote %s\n",val2);
			setLocked(atoi(op1buf),48);
			break;	
	}
	
	
	
}

//simulates one clock cyle			
void executeCycle()
{
	writeBack();
	memAccess();
	execute();
	decode();
	fetch();	
	shiftPipeline();
}



