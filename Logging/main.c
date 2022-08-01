#include <stdio.h>
#include <stdlib.h>
#include "logging.h"

void CallSomeMethod(){

    lgg_log_info("Just ERROR message", error);

}

int main(int argc, char* argv[])
{
    if(argc == 0){
        printf("\nLaunch program error.Execution aborted.\n");
        return 1;
    }

    lgg_LogSys_init(argv[0]);

    lgg_log_info("Just INFO message",    info);
    lgg_log_info("Just DEBUG message",   debug);
    lgg_log_info("Just WARNING message", warning);

    CallSomeMethod();

    lgg_LogSys_deinit();

    return 0;
}
