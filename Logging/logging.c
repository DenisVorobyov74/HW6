
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <execinfo.h>

#define BT_BUF_SIZE 100

FILE *lgg_LogFilePointer;
enum lgg_MsgLvl {info, debug, warning, error};

/////////////////////////////////////////////////////////////////////
////// Внутриннее методы
/////////////////////////////////////////////////////////////////////

// Метод выполняет открытие файлов
static FILE* OpenFile(char* mPathToFile, const char Mode[]) {

    struct stat buff;
    FILE* mStreamPointer = NULL;

    if(Mode[0] == 'w' ||
      (Mode[0] == 'a' && Mode[1] == '+'))
        mStreamPointer = fopen(mPathToFile, Mode);
    else{
        // Проверяем, что введенный путь указывает на обычный файл. Если это файл - попытаемся открыть.
        if(stat(mPathToFile, &buff) == 0 && (buff.st_mode & __S_IFMT) == __S_IFREG)
            mStreamPointer = fopen(mPathToFile, Mode);
    }

    return mStreamPointer;

}

// мметод закрывает файлы
static int CloseFile(FILE* mStreamPointer) {

    int CloseResult;

    if(mStreamPointer == NULL){
        // Возвращаем 0, поскольку проверка корректности указателя должна быть выполнена заранее
        CloseResult = 0;
    }
    else
        CloseResult = fclose(mStreamPointer);

    return CloseResult;

}

static char* GetInfoType(enum lgg_MsgLvl Level){

    char* InfoType;
    switch (Level) {
        case info:
            InfoType = "[INFO]   ";
            break;
        case debug:
            InfoType = "[DEBUG]  ";
            break;
        case warning:
            InfoType = "[WARNING]";
            break;
        case error:
            InfoType = "[ERROR]  ";
            break;
        default:
            perror("Unknown debug level.");
            exit(EXIT_FAILURE);
        break;
    }

    return InfoType;
}
/////////////////////////////////////////////////////////////////////
////// Внешний интерфейс
/////////////////////////////////////////////////////////////////////

extern void lgg_LogSys_init(char Path[]){

    char *LogFileFullName;
    char *LogFileName = "LogFile.txt";

    size_t LogFileLen = strlen(LogFileName);
    size_t PathLen = strlen(Path);
    size_t Indx = PathLen - 1;

    do{
        Indx--;
    }while(Path[Indx] != '/' && Indx != 0);
    LogFileFullName = malloc((Indx + 1 + LogFileLen + 1) * sizeof(char)); // Первый "+1" - конвертация из индекса в длину строки; вторая "+1" - под '\0'

    strncpy(LogFileFullName, Path, Indx+1);
    strncat(LogFileFullName, LogFileName, LogFileLen);

    lgg_LogFilePointer = OpenFile(LogFileFullName, "a+");
    free(LogFileFullName);
}

extern void lgg_LogSys_deinit(){
    CloseFile(lgg_LogFilePointer);
}

extern void lgg_LogMsg(char* MSG, enum lgg_MsgLvl Level, const char* file, const char* func, long line){

    char str_t[128], *InfoType;
    time_t CurrTime = time(NULL);
    strftime(str_t, 128, "%x %X %Z", localtime(&CurrTime));

    InfoType = GetInfoType(Level);
    fprintf(lgg_LogFilePointer, "%s %s - %s --- Location: %s; Method: %s-%d;\n", InfoType, str_t, MSG, file, func, (int)line);

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
            fprintf(lgg_LogFilePointer, "           %s\n", strings[i]);

        free(strings);
    }
}


