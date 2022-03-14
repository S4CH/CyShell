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
#include "signal.h"

void identifySignal(char *input, background_jobs *background_job)
{
    background_jobs copy[20];

    for(int i=0; i<20; i++)
    {
        //copy[i].name = background_job[i].name;
        strcpy(copy[i].name, background_job[i].name);
        copy[i].id = background_job[i].id;
        copy[i].number = background_job[i].number;
    }





    char *process_number = strtok(input, " \t");
    int process = 0;
    for(int i=0; i<strlen(process_number); i++)
    {
        process = process*10 + process_number[i]-'0';
    }

    int pid =-1;
    for(int i=0; i<20; i++)
    {
        /*
        if(background_job->number==process)
        {
            pid = background_job->id;
        }
        */

       if(copy[i].number==process)
       {
           pid = copy[i].id;
           break;
       }
    }
    if(pid==-1 || pid==0)
    {
        printf("No job assigned to that id\n");
        return;
    }

    char *signal = strtok(NULL, " \t");
    int signal_number = 0 ; 
    for(int i=0; i<strlen(signal); i++)
    {
        signal_number = signal_number*10 + signal[i]-'0';
    }
    //printf("Process id--> %d\n",pid);
    //printf("Signal number--> %d\n",signal_number);
    passSignal(pid, signal_number);

}
void passSignal(int pid, int signal)
{
    int ret = 1000;
    if(signal==1)
    {
        ret = kill(pid,SIGHUP);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
    if(signal==2)
    {
        ret = kill(pid,SIGINT);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
    if(signal==3)
    {
        ret = kill(pid,SIGQUIT);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
    if(signal==8)
    {
        ret = kill(pid,SIGFPE);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
    if(signal==9)
    {
        ret = kill(pid,SIGKILL);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
    if(signal==14)
    {
        ret = kill(pid,SIGALRM);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
    if(signal==15)
    {
        ret = kill(pid,SIGTERM);
        if(ret!=0)
        {
            printf("Could not send signal\n");
            return;
        }
        return;
    }
}