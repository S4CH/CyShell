#include "curdir.h"
#include "Headers.h"

int min(int a, int b)
{
    if(a<b)
    {
        return a;
    }
    return b;
}

void curdir(char *startDir)
{
    
    char curDir[1024];
    getcwd(curDir,1024);
    int start = 0;
    int end = strlen(curDir);
    int len_start = strlen(startDir);
    int is_same = 1;
    if (end<len_start)
    {
        printf("\033[1;30m");
        printf("%s", curDir);
        printf("\033[0m");
        printf("> ");
    }

    else
    {
        int minimum = min(end,len_start);
        for(int i=0; i<minimum; i++)
        {
            if (curDir[i] != startDir[i])
            {
                is_same = 0;
                break;
            }
        }
        if (is_same)
        {
            printf("\033[1;30m");
            printf("~");
            //for (int i = len_start; i < end; i++)
            int i=len_start;
            while(i<end)
            {
                printf("%c", curDir[i]);
                i++;
            }
            printf("\033[0m");
            printf("> ");
        }
        else
        {
            printf("\033[1;30m");
            for(int i=0 ;i<end; i++)
            {
                printf("%c",curDir[i]);
            }
            //printf("%s", curDir);
            printf("\033[0m");
            printf("> ");
        }
    }
}