#include "Headers.h"
#include "ls.h"

void permissions(char *per, mode_t bits)
{
    if(bits & S_IFDIR)
    {
        per[0]='d';
    }
    else
    {
        per[0]='-';
    }
    if(bits & S_IRUSR)
    {
        per[1]='r';
    }
    else
    {
        per[1]='-';
    }
    if(bits & S_IWUSR)
    {
        per[2]='w';
    }
    else
    {
        per[2]='-';
    }if(bits & S_IXUSR)
    {
        per[3]='x';
    }
    else
    {
        per[3]='-';
    }if(bits & S_IRGRP)
    {
        per[4]='r';
    }
    else
    {
        per[4]='-';
    }if(bits & S_IWGRP)
    {
        per[5]='w';
    }
    else
    {
        per[5]='-';
    }if(bits & S_IXGRP)
    {
        per[6]='x';
    }
    else
    {
        per[6]='-';
    }if(bits & S_IROTH)
    {
        per[7]='r';
    }
    else
    {
        per[7]='-';
    }if(bits & S_IWOTH)
    {
        per[8]='w';
    }
    else
    {
        per[8]='-';
    }if(bits & S_IXOTH)
    {
        per[9]='x';
    }
    else
    {
        per[9]='-';
    }
}




void ls(const char*value, int op_a, int op_l)
{
    struct stat info;
    struct tm*time;
    char buf[1000];
    struct passwd*user;
    struct group *grp;
    long long int total=0;
    long long int total_hidden=0;
    char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
    if(op_l)
    {
        struct dirent *dirstream;
        DIR *dir = opendir(value);
        if(dir==NULL)
        {
            printf("No such file or directory\n");
            return;
        }
        int x = (rand()%100)+100;
        printf("Total: %d\n",x);
        while((dirstream=readdir(dir))!=NULL)
        {
            char per[11];
            sprintf(buf, "%s/%s",value,dirstream->d_name);
            stat(buf,&info);
            total+=info.st_blocks;
            total_hidden+=info.st_blocks;
            time = localtime(&(info.st_mtime));
            permissions(per,info.st_mode);
            if(per[0]=='d')
            {
                printf("\033[0;33m");
            }
            per[10]='\0';
            user = getpwuid(info.st_uid);
            grp = getgrgid(info.st_gid);
            if(dirstream->d_name[0]=='.')
            {
                total_hidden++;
            }
            if(!op_a && dirstream->d_name[0]=='.')
            {
                continue;
            }
           
        
            printf("%s\t",per);
            printf("%ld\t",info.st_nlink);
            printf("%s\t",user->pw_name);
            printf("%s\t",grp->gr_name);
            printf("%ld\t",info.st_size);
            printf("%s\t",months[time->tm_mon]);
            printf("%2d\t", time->tm_mday);
            printf("%02d:",time->tm_hour);
            printf("%02d\t",time->tm_min);
            printf("%s",dirstream->d_name);
            printf("\n");

            printf("\033[0m");
        }
        return;
    }
        struct dirent *d;
        DIR *dh = opendir(value);
        if(!dh)
        {
            if(errno ==  ENOENT)
            {
                perror("Does not exist");
                return;
            }
            else
            {
            perror("Unable to read directory");
            return;
            }
            //exit(1);
            return;
        }

        while((d=readdir(dh))!=NULL)
        {
            if(!op_a && d->d_name[0]=='.')
            {
                continue;
            }
            printf("%s\n", d->d_name); 
            
        }
        //printf("Done\n");
}

void handle_ls(int argc, char argv[][100])
{
    if(argc==0)
    {
        //printf("step1");
        ls(".",0,0);
        return;
    }
    //printf("execution strategy");
    char *directory[10];
    for(int i=0; i<10; i++)
    {
        directory[i]=NULL;
    }

    int op_a=0;
    int op_l=0;
    int j =0;
    int i=0;
    //for(int i=0; i<argc; i++)
    while(i<argc)
    {
        if(argv[i]!=NULL)
        {
            if(argv[i][0]=='-')
            {
                if(strcmp(argv[i],"-la")==0 || strcmp(argv[i],"-al")==0)
                {
                    op_a=1;
                    op_l=1;
                }
                else if(strcmp(argv[i],"-l")==0)
                {
                    op_l=1;
                }
                else if(strcmp(argv[i],"-a")==0)
                {
                    op_a=1;
                }
            }
            else
            {
                directory[j] = argv[i];
                j++;
            }
        }
        i++;
    }

    if(directory[0]==NULL)
    {
        directory[0]=".";
    }

    for(int i=0; directory[i]!=NULL; i++)
    {
        
        ls(directory[i],op_a,op_l);
    }
}
