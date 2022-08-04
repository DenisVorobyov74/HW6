#include <stdio.h>
#include <stdlib.h>
#include "libLoggingLib.h"

void CallSomeMethod(){

    lgg_LogErr("Just ERROR message");

}

int main(int argc, char* argv[])
{
    if(argc == 0){
        printf("\nLaunch program error.Execution aborted.\n");
        return 1;
    }

    lgg_LogSys_init(argv[0]);

    lgg_LogInfo("Just INFO message");
    lgg_LogDbg ("Just DEBUG message");
    lgg_LogWrng("Just WARNING message");

    CallSomeMethod();

    lgg_LogSys_deinit();

    return 0;
}
