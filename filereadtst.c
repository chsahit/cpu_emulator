#include <stdio.h>
		
FILE *fp;

void readExp(char buf[28])
{
	fread(buf,1,27,fp);
	buf[27] = (char)0;
	printf(buf);
}

int main()
{
	fp = fopen("testfile.sasm.o","r");
	char line[28];
	int index = 0;
	int i;
	
	for(i = 0; i < 3; i++) {
		readExp(line);
		fseek(fp,27,index);
		index = index + 27;
	}
	index = 0;
	readExp(line);
	fseek(fp,27,index);
	readExp(line);
	fseek(fp,27,index);
	
	
	return 0;
}

