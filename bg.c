#include "Headers.h"
#include "parse.h"
#include "echo.h"
#include "ls.h"
#include "cd.h"
#include "pwd.h"
#include "Username.h"
#include "curdir.h"
#include "pinfo.h"
#include "history.h"
#include "jobs.h"
#include "signal.h"
#include "bg.h"

void handle_bg(background_jobs *background_job, char *input)
{
    int number = 0;
    //printf("%ld\n",strlen(input));
    for(int i=0; i<strlen(input); i++)
    {
        number = number*10 + input[i]-'0';
    }

    //printf("number-->%d\n",number);


    int pid = -1;
    for(int i=0; i<20; i++)
    {
        if(background_job[i].number == number)
        {
            pid = background_job[i].id;
            //rintf("%d-->%d\n", number, pid);
            break;
        }
    }
    if(pid==-1 || pid==0)
    {
        printf("No background job for that number\n");
        return;
    }
    bg(pid);
    
}

void bg(int pid)
{
    if (kill(pid, SIGCONT) < 0)
    {
        perror("error :sending the signal");
        return;
    }


}