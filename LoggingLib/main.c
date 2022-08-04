
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <execinfo.h>
#include <libgen.h>

#define BT_BUF_SIZE 100

FILE *lgg_LogFilePointer;
enum lgg_MsgLvl {info, debug, warning, error};

/////////////////////////////////////////////////////////////////////
////// Внетренний интерфейс
/////////////////////////////////////////////////////////////////////

size_t strlcpy(char *dst, const char *src, size_t dsize) {

	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}

	return(src - osrc - 1);	/* count does not include NUL */
}

/////////////////////////////////////////////////////////////////////
////// Внешний интерфейс
/////////////////////////////////////////////////////////////////////

void lgg_LogSys_init(char Path[]){

    char *LogFileFullName;
    char *LogFileName = "/LogFile.txt";

    size_t LogFileLen = strlen(LogFileName)+1;
    char*  FolderFullPath = dirname(Path);
    size_t PathLen = strlen(FolderFullPath)+1;

    LogFileFullName = malloc((PathLen + LogFileLen + 1) * sizeof(char)); // "+1" - под '\0'

    strlcpy(LogFileFullName, Path, PathLen);
    strlcpy(LogFileFullName+PathLen-1, LogFileName, LogFileLen);

    lgg_LogFilePointer = fopen(LogFileFullName, "a+");
    free(LogFileFullName);

}

void lgg_LogSys_deinit(){
    fclose(lgg_LogFilePointer);
}

void lgg_LogMsg(char* MSG, enum lgg_MsgLvl Level, const char* file, const char* func, long line){

    char str_t[128];
    time_t CurrTime = time(NULL);
    strftime(str_t, 128, "%x %X %Z", localtime(&CurrTime));

    static char InfoType[4][10] = {
        {"[INFO]   "},
        {"[DEBUG]  "},
        {"[WARNING]"},
        {"[ERROR]  "}};

    fprintf(lgg_LogFilePointer, "%s %s - %s --- Location: %s; Function: %s-%d;\n", InfoType[Level], str_t, MSG, file, func, (int)line);

    if(Level == error){

        // For more info read: https://man7.org/linux/man-pages/man3/backtrace.3.html

        int nptrs;
        void *buffer[BT_BUF_SIZE];
        char **strings;

        nptrs = backtrace(buffer, BT_BUF_SIZE);
        //printf("backtrace() returned %d addresses\n", nptrs);

        /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
           would produce similar output to the following: */

        strings = backtrace_symbols(buffer, nptrs);
        if (strings == NULL) {
            perror("backtrace_symbols");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nptrs; i++)
            fprintf(lgg_LogFilePointer, "               %s\n", strings[i]);

        free(strings);
    }
}
