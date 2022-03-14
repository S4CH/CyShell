#include "Headers.h"
#include "pwd.h"

void pwd()
{
    char path[1024];

    getcwd(path,1024);
    printf("%s\n",path);
    return;
}

