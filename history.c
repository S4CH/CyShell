#include "Headers.h"
#include "history.h"
#include "curdir.h"

void Print_History(int num, char*start_dir)
{
    char path[150];
    strcpy(path,start_dir);
    strcat(path, "/history.txt");
    char c;
    int count=0;
    FILE *fp;
    fp = fopen(path,"r");
    if(fp==NULL)
    {
        perror("Does not exist");
    }
    for(c = getc(fp); c!=EOF; c=getc(fp))
    {
        if(c=='\n')
        {
            count++;
        }
    }
    fclose(fp);

    if(count<num)
    {
        printf("Only %d commands stored\n", count);
    }
    fopen(path,"r");
    int skip =count-num;
    int print_till = min(num,count);
    for(int i=0;i<count; i++)
    {
        char str[200];
        strcpy(str,"\0");
        
        fgets(str,200,fp);
        if(i>=skip)
        {
            //printf("Here......");
            printf("%s",str);
        }
    }
    fclose(fp); 

}



void Write_History(char *start_dir,char* command)
{
    char path[150];
    strcpy(path,start_dir);
    strcat(path, "/history.txt");
    char c;
    int count=0;
    FILE *fp;
    fp = fopen(path,"a");
    if(fp==NULL)
    {
        printf("failed\n");
        return;
    }
    fprintf(fp,"%s",command);
    fclose(fp);

    fp = fopen(path,"r");
    for(c = getc(fp); c!=EOF; c=getc(fp))
    {
        if(c=='\n')
        {
            count++;
        }
    }
    fclose(fp);
    if(count>20)
    {
        char str[1000];
        char temp[150];
        strcpy(temp,start_dir);
        strcat(temp,"/temp.txt");
        FILE *gp = fopen(temp,"a");
        fp = fopen(path,"r");
        int line_no=1;
        int ctr=0;
        while(!feof(fp))
        {
            strcpy(str,"\0");
            fgets(str,1000,fp);
            if(!feof(fp))
            {

                ctr++;
                if(ctr!=line_no)
                {
                    //printf("printing\n");
                    fprintf(gp,"%s",str);
                }
            }
        }
        fclose(fp);
        fclose(gp);
        remove(path);
        rename(temp,path);
    }
}

bool Check_History(char*start_dir,char*command)
{
    char path[200];
    strcpy(path,start_dir);
    strcat(path,"/history.txt");
    char str[300];
    FILE* fp = fopen(path,"r");
    if(!fp)
    {
        return true;
    }
    char temp[300];
    while(!feof(fp))
    {
        strcpy(temp,str);
        strcpy(str,"\0");
        fgets(str,100,fp);
    }

    char command_copy[1000];
    strcpy(command_copy,command);
    if(strcmp(temp,command_copy) == 0)
    {
        return false;
    }
    return true;
}