extern void executeCycle();
extern void initCPU();
extern void initMem();
extern int getInstrLine();
extern void setInstrLine(int line);
extern void getValAtAddr(int addr,char buf[9]);
extern void setValAtAddr(int addr,char buf[9]);
extern int isLocked();
extern void setLocked(int addr,int locked);