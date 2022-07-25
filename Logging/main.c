#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logging.h"

int main(int argc, char* argv[])
{
    if(argc == 0){
        printf("\nLaunch program error.Execution aborted.\n");
        return 1;
    }

    lgg_LogSys_init(argv[0]);

    lgg_log_info("Just INFO message", ctime(time(NULL)));

    lgg_LogSys_deinit();

    return 0;
}
