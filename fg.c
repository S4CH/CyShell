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
#include "fg.h"

bool handle_fg(background_jobs *background_job, char *input, int *fg_process_id)
{
    int number = 0;
    for(int i=0; i<strlen(input); i++)
    {
        number = number*10 + input[i]-'0';
    }
    char fg_name[100];

    int pid = -1;
    for(int i=0; i<20; i++)
    {
        if(background_job[i].number == number)
        {
            pid = background_job[i].id;
            //printf("%d-->%d\n", number, pid);
            strcpy(fg_name, background_job[i].name);
            background_job[i].id = 0;
            background_job[i].name[0]='\0';
            break;
        }
    }
    if(pid==-1 || pid==0)
    {
        printf("No background job for that number\n");
        return false;
    }
    

    return fg(pid,fg_name, fg_process_id);
}
bool fg(int pid, char*process_name, int *fg_process_id)
{
    int status;
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0,getpgid(pid));
    if (kill(pid, SIGCONT) == -1)
    {
        printf("error: could not send signal\n");
        return false;
    }
    *fg_process_id = pid;
    //printf("%d\n", *fg_process_id);
    waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpgid(getpid()));
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    

}