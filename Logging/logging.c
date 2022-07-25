
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

FILE *lgg_LogFilePointer;

//#define lgg_log_info(MSG, TIME) fprintf(lgg_LogFilePointer, "[INFO] " TIME ": -- " MSG "\n");

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
