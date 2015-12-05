extern void executeCycle();
extern void initCPU();
extern void initMem();
extern int getInstrLine();
extern void setInstrLine(int line);
extern void getValAtAddr(int addr,char buf[9]);
extern void setValAtAddr(int addr,char buf[9]);
extern int getLocked();
extern void setLocked(int addr);