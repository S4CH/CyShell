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

void command(char*string, int background,background_jobs *background_job, int *background_count, char *present, int *fg_process_id)
{
    int standardIn = dup(STDIN_FILENO);
    int standardOut =  dup(STDOUT_FILENO);
    char copy[100];
    char copy2[100];
    strcpy(copy2, string);
    strcpy(copy, string);
    int len = strlen(string);
    int status=1;
    if(string[len-1] == '&' || string[len-2] == '&')
    {
        background=1;
        string[len-2]='\0';
        command(string,background,background_job, background_count,present, fg_process_id);
        return;
    }
    char *order = strtok(copy2, " \t");
    if(strcmp(order, "history")==0)
    {
        char *count = strtok(NULL, " ");
        if(count!=NULL)
        {
            int i=strlen(count);
            int value = 0;
            for(int j=0; j<i; j++)
            {
                value = value*10 + count[j]-'0';
            }
            Print_History(value,present);
        }
        else
        {
            Print_History(10,present);
        }
    }
    else if(strcmp(order, "echo")==0)
    {
        char *input = strtok(NULL, "\n");
        echo_str(input);
    }
    else if(strcmp(order, "repeat")==0)
    {
        char *number = strtok(NULL, " \t");
        int t = strlen(number);
        int count=0;
        for(int i=0; i<t; i++)
        {
            int current = number[i]-'0';
            count = count*10 + current;
        }
        //printf("%d\n",count);

        char try[count][1000];
        char *remaining_command = strtok(NULL,"\n");
        if(remaining_command!=NULL)
        {
            for(int i=0; i<count; i++)
            {
                strcpy(try[i],remaining_command);
            }
            for(int i=0; i<count; i++)
            {
                //command(remaining_command,background,background_jobs,background_count);
                command(try[i],background,background_job,background_count,present, fg_process_id);
            }
        }
        
        return;

    }
    else if(strcmp(order,"ls")==0)
    {
        
        char ls_input[10][100];
        int count = 0;
        char *input = strtok(NULL," ");
        while(input!=NULL)
        {
            if(strcmp(input,">")==0 || strcmp(input,">>")==0)
            {
                break;
            }
            //printf("%s3\n",input);
            count++;
            strcpy(ls_input[count-1],input);
            //printf("%s4\n",ls_input[count-1]);
            input = strtok(NULL, " ");
        }
        //printf("%d_5\n",count);
       
        handle_ls(count,ls_input);   
    }
    else if(strcmp(order,"cd")==0)
    {
        char *input = strtok(NULL, "\n");
        if(input == NULL)
        {
            input = "~";
        }
        char *prev = getenv("PWD");
        char prev_s[100];
        //strcpy(prev_s,prev);
        //printf("%s\n",prev_s);
        handle_cd(input, prev,present);
    }
    else if(strcmp(order,"pwd")==0)
    {
        pwd();
    }
    else if(strcmp(order,"pinfo")==0)
    {
        char *input = strtok(NULL, " \n");

        //printf("%s         %s      %s       %s----> %ld\n", input,input,input,input, strlen(input));

        if(input==NULL)
        {
            int pid = getpid();
            pinfo(pid);
        }
        else
        {
            int pid = 0;
            for(int i=0; i<strlen(input); i++)
            {
                int value = input[i]-'0';
                pid = pid*10 +value;
            }
            pinfo(pid);
        }
    }
    else if(strcmp(order,"quit")==0 || strcmp(order,"exit")==0)
    {
        exit(1);
    }
    else if(strcmp(order,"jobs")==0)
    {
        char *job_flag = strtok(NULL,"\n");
        print_jobs(background_job, job_flag);
    }
    else if(strcmp(order, "sig")==0)
    {
        char *input = strtok(NULL,"\n");
        identifySignal(input, background_job);

    }
    else if(strcmp(order, "bg")==0)
    {
        char *input = strtok(NULL,"\n");
        //printf("%s\n",input);
        handle_bg(background_job, input);
    }
    else if(strcmp(order, "fg")==0)
    {
        char *input = strtok(NULL,"\n");
        //printf("%s\n",input);
        if(handle_fg(background_job, input, fg_process_id))
        {
            (*background_count)--;
        }
    }
    else if(strcmp(order, "replay")==0)
    {
        char *input = strtok(NULL,"\n");
        char check[1000];
        strcpy(check,input);
        int interval = 0;
        int period = 0;
        char comm[1000];
        char *steps = strtok(check, "\t ");
        while(steps!=NULL)
        {
            if(strcmp(steps,"-interval")==0 || strcmp(steps,"interval")==0)
            {
                steps = strtok(NULL," \t\n");
                int x = strlen(steps);
                for(int i=0; i<x; i++)
                {
                    interval =interval*10 + steps[i]-'0';
                }
            }
            if(strcmp(steps,"-period")==0 || strcmp(steps,"period")==0)
            {
                //printf("ENTER LLOPP\n");
                steps = strtok(NULL," \t\n");
                int x = strlen(steps);
                for(int i=0; i<x; i++)
                {
                    period =period*10 + steps[i]-'0';
                 //   printf("%d\n",period);
                }
            }
            
            if(strcmp(steps,"-command")==0)
            {
                steps = strtok(NULL,"-\n\t");
                strcpy(comm,steps);
                
            }
            steps =  strtok(NULL," \t\n");
            //printf("%s\n",steps);
        }
        int time_elapsed=0;
        while(time_elapsed<period)
            {
                command(comm, background, background_job, background_count, present, fg_process_id);
                sleep(interval);
            time_elapsed+=interval;
            }
    }


    else
    {
        char *input = strtok(copy, "\n");
        char *argv[100];
        int i=0;
        char *next = strtok(input," \t");
        while(next !=NULL)
        {
            argv[i]=next;
            next = strtok(NULL," \t");
            i++;
        }
        argv[i]=NULL;

        if(background==0)
        {
            pid_t pid = fork();
            if(pid<0)
            {
                perror("Child not created");
                return;
            }
            else if(pid>0)
            {
                *fg_process_id=pid;
                int w_st;
                setpgid(pid,0);
                waitpid(pid, &w_st,WUNTRACED);
            }
            else
            {
                setpgid(0,0);
                //printf("%s100\n",input);
                int len = strlen(argv[0]);
                //dup2(standardIn,STDIN_FILENO);
                //dup2(standardOut,STDOUT_FILENO);
                //printf("%s %s %s\n",argv[0],argv[1],argv[2]);

                
                if(execvp(argv[0],argv)<0)
                {
                    perror("Could not execute command");
                    exit(1);
                }

                //wait(NULL);
            }
        }
        else
        {
            //printf("BGP11\n");
            pid_t pid = fork();

            if(pid<0)
            {
                perror("Could not execute command");
                return;
            }
            else if(pid>0)
            {
                setpgid(pid,0);

                background_job[*(background_count)].id=pid;
                background_job[*(background_count)].number = *background_count;
                strcpy(background_job[*(background_count)].name, order);

                //printf("%d\n",pid);

                (*background_count)++;
                (*background_count)=(*background_count)%20;
                //printf("%ls\n",background_count); 
                printf("%d\n",pid);
                //printf("%d-->",*background_count);
                
                return;
            }
            else
            {
                setpgid(0,0);
                if(execvp(argv[0],argv)<0)
                {
                    perror("Could not execute command");
                    exit(1);
                }
            }
            
    }
    
}
}


void check_commands(char*input, background_jobs* background_job,int*background_count,char *present, int *fg_process_id)
{
    int number_of_semicolon=0;
    for(int i=0; i<strlen(input); i++)
    {
        if(input[i]==';')
        {
            number_of_semicolon++; 
        }
    }

    char commands[number_of_semicolon+5][100];
    char*next = strtok(input,";");
    int i=0;
    while(next!=NULL)
    {
        strcpy(commands[i],next);
        
        next = strtok(NULL,";");
        i++;
    }

    
    for(int j=0; j<i; j++)
    {
        int standardIn = dup(STDIN_FILENO);
        int standardOut =  dup(STDOUT_FILENO);
        char tester[1000];
        strcpy(tester,commands[j]);
        char temp_command[50][100];
        
        //strcpy(temp_command,commands[j]);
        int i=0;
        char *next = strtok(tester, " \t");
        while(next!=NULL)
        {
            strcpy(temp_command[i],next);
            next = strtok(NULL," \t");
            i++;
        }

        bool is_pipe_present = false;
        int number_of_pipes = 0;
        for(int k=0; k<i; k++)
        {
            if(strcmp(temp_command[k], "|")==0)
            {
                number_of_pipes++;
                is_pipe_present = true;
            }   
            
            if(strcmp(temp_command[k], "<")==0)
            {
                if(temp_command[k+1]!=NULL)
                {
                    printf("%s\n",temp_command[k+1]);
                    int fd = open(temp_command[k+1],O_RDONLY);
                    if(fd<0)
                    {
                        dup2(standardIn,STDIN_FILENO);
                        dup2(standardOut,STDOUT_FILENO);
                        printf("File does not exist for input\n");
                        return;
                    }
                    dup2(fd,STDIN_FILENO);
                    close(fd);   
                }
                else
                {
                    printf("Not enough arguements\n");
                }
            }



            if(strcmp(temp_command[k], ">")==0 || strcmp(temp_command[k], ">>")==0)
            {
                //printf("FAIL1\n");
                if(temp_command[k+1]!=NULL)
                {
                    int fd;
                    //int fd = open(temp_command[k+1],O_RDONLY);
                    if(strcmp(temp_command[k],">")==0)
                    {
                        fd = open(temp_command[k+1],O_CREAT| O_TRUNC | O_WRONLY, 0644);
                        //printf("%d\n",fd);
                        
                    }
                    else
                    {
                        fd = open(temp_command[k+1],O_CREAT | O_WRONLY | O_APPEND, 0644);
                        
                    }
                    if(fd<0)
                    {
                        
                        dup2(standardIn,STDIN_FILENO);
                        dup2(standardOut,STDOUT_FILENO);
                        perror("Open:");
                        printf("File does not exist for output\n");
                        return;
                    }
                    dup2(fd,STDOUT_FILENO);

                    
                    close(fd);   
                }
                else
                {
                    printf("Not enough arguements\n");
                }
            }

        }

        int fds[2];
        pipe(fds);
        
        
        char *piped_command = strtok(commands[j], "|");
        while(piped_command!=NULL)
        {
            if(is_pipe_present)
            {
                dup2(fds[1], STDOUT_FILENO);
                number_of_pipes--;
            }
            char pipes[1000];
            strcpy(pipes,piped_command);
            piped_command = strtok(NULL,"|");
            char *filtered_command = strtok(pipes, "<>");
            command(filtered_command,0,background_job,background_count,present, fg_process_id);
            if(is_pipe_present)
            {
                dup2(standardOut,STDOUT_FILENO);
                if(dup2(fds[0],STDIN_FILENO)<0)
                {
                    printf("could not change input\n");
                }
                if(number_of_pipes==0)
                {
                    is_pipe_present=false;
                    printf("No more pipes\n");
                }
            }

        }

        dup2(standardIn,STDIN_FILENO);
        dup2(standardOut,STDOUT_FILENO);
    }
}

