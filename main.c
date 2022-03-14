#include "Headers.h"
#include "Username.h"
#include "pwd.h"
#include "cd.h"
#include "ls.h"
#include "echo.h"
#include "parse.h"
#include "curdir.h"
#include "pinfo.h"
#include "history.h"
#define start_dir getenv("PWD")

int background_count=0;
background_jobs background[20];

int fg_process_id = -1;
char fg_process_name[100];




void Handler(int p)
{
    fflush(stdout);
    for(int i=0; i<20; i++)
        {
            int status;
            pid_t return_pid = waitpid(background[i].id, &status, WNOHANG);
            if(return_pid == background[i].id && return_pid == background[i].id!=0)
            {
                printf("%s Process with pid %d exited ",background[i].name , background[i].id);
                background[i].id = 0;
                background[i].name[0]='\0';
                background_count--;
                if(status == 0)
                {
                    printf("Normally\n");
                    fflush(stdout);
                    Username();
                    curdir(start_dir);
                }
                else
                {
                    printf("Abnormally\n");
                    Username();
                    curdir(start_dir);
                    fflush(stdout);
                }
            }
        }
}

void Push_to_background(int p)
{
    if(fg_process_id!=-1)
    {
        kill(fg_process_id, SIGTSTP);

        //strcpy(background[background_count].name, "Foreground Process");
        strcpy(background[background_count].name,fg_process_name);
        background[background_count].number = background_count;
        background[background_count].id = fg_process_id;
        background_count++;
    }
    printf("\n");
    Username();
    curdir(start_dir);
    fflush(stdout);
}

void KillForegroundProcess(int p)
{
    //kill(getpid(),SIGINT);

    if(fg_process_id!=-1)
    {
        kill(fg_process_id,SIGINT);
        fg_process_id=-1;
    }
    printf("\n");
    Username();
    curdir(start_dir);
    fflush(stdout);
}

void exit_handler(int p)
{
    exit(1);
}

int main()
{
    for(int i=0; i<20; i++)
    {
        background[i].id = 0;
    }
    char pwd[1000];
    strcpy(pwd,start_dir);
    char str[100][256];
    int status = 1;
    char *in_line;
    ssize_t len = 0;
    ssize_t buffer_size=1000;
    signal(SIGCHLD, Handler);
    signal(SIGINT, KillForegroundProcess);
    //signal(SIGTSTP, KillForegroundProcess);
     signal(SIGTSTP, Push_to_background);
     while(1)
    {
        fflush(stdout);
        len=0;
        Username();
        curdir(pwd);
        //signal(SIGCHLD, Handler);
        //signal(SIGINT, Continue);
        len = getline(&in_line,&buffer_size,stdin);
        if(Check_History(pwd,in_line))
        {
            Write_History(pwd,in_line);
        }

        if(len==-1)
        {
            printf("\033[0:31m");
            printf("THANK YOU\n");
            exit(1);
        }
        
        if(in_line[len-1]=='\n')
        {
            in_line[len-1]='\0';
        }
        strcpy(fg_process_name, in_line);
        check_commands(in_line,background,&background_count,pwd, &fg_process_id);
        in_line[len-1]='\n';


        fflush(stdout);
    }
}