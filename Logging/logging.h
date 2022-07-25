/*#ifndef LOGGING_INCLUDED
#define LOGGING_INCLUDED



#endif // LOGGING_INCLUDED*/

//FILE *lgg_LogFilePointer;

#define lgg_log_info(MSG, TIME) fprintf(lgg_LogFilePointer, "[INFO] " TIME " -- " MSG "\n");

static FILE* OpenFile(char* mPathToFile, const char Mode[]);
static int CloseFile(FILE* mStreamPointer);

extern void lgg_LogSys_init(char LogFileName[]);
extern void lgg_LogSys_deinit();

