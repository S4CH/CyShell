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

int comparator(const void *p, const void *q)
{
    background_jobs *element1 = (background_jobs *)p;
    background_jobs *element2 = (background_jobs *)q;

    if(element1->name[0] < element2->name[0])
    {
        return -1;
    }
    if(element1->name[0] > element2->name[0])
    {
        return 1;
    }
    return 0;
}



void print_jobs(background_jobs *background_job, char *flags)
{
    background_jobs copy[20];

    for(int i=0; i<20; i++)
    {
        strcpy(copy[i].name, background_job[i].name);
        copy[i].id = background_job[i].id;
        copy[i].number = background_job[i].number;

    }

    int running=0;
    int stopped=0;

    if(flags==NULL)
    {
        running=1;
        stopped=1;
    }
    else
    {
        char *check_flags = strtok(flags," \t");
        while(check_flags!=NULL)
        {
            if(strcmp(check_flags, "-r")==0)
            {
                running=1;
            }
            if(strcmp(check_flags,"-s")==0)
            {
                stopped=1;
            }
            if(strcmp(check_flags,"-sr")==0 || strcmp(check_flags,"-rs")==0)
            {
                running=1;
                stopped=1;
            }
            check_flags=strtok(NULL," \t");
        }
    }

    

    
    char line[100];
    char *status;
    FILE *fd;
    qsort(copy,20, sizeof(copy[0]),comparator);    

    char *path;
    for (int i = 0; i < 20; i++)
    {
        if(copy[i].id!=0)
        {
            path = (char*)malloc(50*sizeof(char));
            snprintf(path, 50, "/proc/%d/stat", copy[i].id);
            fd = fopen(path, "r");

            for (int i = 0; i < 23; i++)
            {
                if (i == 2)
                {
                    fscanf(fd, "%s", line);
                    status = (char *)malloc(10);
                    strcpy(status, line);
                    break;
                }
                else
                {
                    fscanf(fd, "%s", line);
                }
            }
            

            if(strcmp(status,"T")==0 && stopped==1)
            {
                printf("[%d] %s %s [%d]\n",copy[i].number, "Stopped", copy[i].name, copy[i].id);
            }
            if(strcmp(status,"T")!=0 && running==1)
            {
                printf("[%d] %s %s [%d]\n",copy[i].number, "Running", copy[i].name, copy[i].id);
            }

        
        }
    }
}