#include "Headers.h"
#include "pinfo.h"


void pinfo(pid_t pid)
{
    char path[50];
    char line[100];
    long long vm;
    long long fgpid;
    long long gpid;
    char *status;
    FILE *fd;

    printf("pid -- %d\n", pid);

    snprintf(path, 50, "/proc/%d/status", pid);
    printf("%s\n",path);

    fd = fopen(path, "r");
    if (!fd)
    {
        perror("fopen:");
        return;
    }

    int j=0;
    while(j<23)
    {
        if(j==5)
        {
            char *line_new=NULL;
            size_t len=0;
                
            fscanf(fd,"%s",line);
            
            status = (char*)malloc(50*sizeof(char));
            strcpy(status,line);
            printf("Process Status -- %s", status);
            
        }
        if(j==4)
        {
            fscanf(fd,"%lld", &gpid);
        }
        if(j==7)
        {
            fscanf(fd,"%lld", &fgpid);
            if (fgpid == gpid)
            {
                printf("+\n");
            }
            else
            {
                printf("\n");
            }
        }
        if(j==22)
        {
            fscanf(fd,"%lld", &vm);
            printf("memory -- %lld\n", vm);
        }
        fscanf(fd,"%s",line);
        j++;
    }

    fclose(fd);
    
    
    snprintf(path, 50, "/proc/%d/exe", pid);
    
    printf("%s\n",path);
    
    ssize_t len = readlink(path, line, 100);
    
    line[len] = '\0';
    
    if (len != -1)
    {
        printf("Executable Path -- %s\n", line);
    }
    
    /*else
    {
        printf("Can't read execute path of file\n");
    }*/
    return;
}