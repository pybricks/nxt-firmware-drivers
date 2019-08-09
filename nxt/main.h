#ifndef MAIN_H
#define MAIN_H
extern unsigned int gNextProgramSize;
extern byte *gNextProgram;
extern unsigned int gProgramExecutions;

extern int run_program();
extern void shutdown(int update_mode);
extern void firmware_exception_handler();
#endif
