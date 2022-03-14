#include "Headers.h"
#include "cd.h"
#include "pwd.h"

void change(char*relative,char*path,int start)
{
    int len=strlen(path);
    if(len>2)
    {
        for(int i=start; i<len; i++)
        {
            relative[i] = path[i];
        }
        chdir(relative);
    }
}

void cd(char*path, char*home, char*prev)
{
    //printf("%s\n",prev);
    char relative[1024]="./";
    char root = '/';
    char dot = '.';
    char tilde ='~';
    char dash = '-';

    if(path[0]==dash)
    {
        char temp[1000];
        strcpy(temp,prev);
        prev =getcwd(prev,100);
        chdir(temp);
    }
    else
    {   
        char *path_new = strtok(path," \t");
        //printf("x%sx",path_new);
        prev = getcwd(prev,100);
        if(path[0]== tilde)
        {
            chdir(home);
            change(relative,path_new,2);
        }

        else if(path_new[0]==dot && path_new[1]!=dot)
        {
            change(relative,path_new,0);
        }
        else if(chdir(path_new)==-1)
        {
            perror("CHDIR:");
        }
    }
}

void handle_cd(char *path, char *prev,char *start)
{
    //char *start = getenv("PWD");
    cd(path,start,prev);
}