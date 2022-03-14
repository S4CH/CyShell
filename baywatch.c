#include "Headers.h"
//#include "baywatch.h"
#include "parse.h"

char getch();
char * read_file(char *filename, int n);
void baywatch(char **args, int no_args, background_jobs *background_job, int background_count);
void dirty();
void newborn(background_jobs *background_job, int background_count);
void interrupt();



char getch() 
{
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}



char * read_file(char *filename, int n)
{   
    FILE* file = fopen(filename, "r");
    char line[256];
    char *info = malloc(sizeof(char) *256);
    int i = 0;
    while (fgets(line, sizeof(line), file)) 
    {
        i++;
        if(i == n)
            strcpy(info, line);
    }

    fclose(file);
    return info;
}


void baywatch(char **args, int no_args, background_jobs *background_job, int background_count)
{
    int f = 0;
    if(no_args !=3 )
    {
        printf("Kindly give input as `nightswatch -n [seconds] [dirty/interrupt/newborn]\n");
        return;
    }

    
    else if(!strcmp(args[2], "dirty"))
    {
        f = 1;
    }
    
    else if(!strcmp(args[2], "interrupt"))
    {
        f = 0;
    }
    else if(strcmp(args[2], "newborn")==0)
    {
        f=2;
    }

    else 
    {
        printf("Kindly give input as `nightswatch -n [seconds] [dirty/interrupt/newborn]\n");
        return;
    } 

    int n = atoi(args[1]);
    
    if(f==0)
    {
        FILE *foo = fopen("/proc/interrupts", "r");
        if(foo == NULL)
            printf("Can't open /proc/interrupts file\n");

        else
        {
            char *line = read_file("/proc/interrupts", 1);
            printf("%s\n",line);
            free(line);  
            fclose(foo);
        }

    }
    
    while(1)
    {
        fd_set rfds;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int retval; FD_ZERO(&rfds); FD_SET(0, &rfds);

        retval = select(1, &rfds, NULL, NULL, &tv);

        if (retval == -1)
            perror("select()");
        
        else if (retval)
        {
            retval = select(1, &rfds, NULL, NULL, &tv);
            char c = getch(stdin);

            if(c == 'q')
                return;
        }
            
        if(f == 0)
        {
            interrupt();
        }
        else if(f==1)
        {
         dirty();
        }
        else
        {
            newborn(background_job, background_count);
        }
        sleep(n);   
    }
}

void dirty()
{
    FILE *f = fopen("/proc/meminfo", "r");
    if(f == NULL)
        printf("Can't open /proc/meminfo file\n");

    else
    {
        char *line = read_file("/proc/meminfo", 17);
        printf("%s\n",line);
        free(line);  
    }

    fclose(f);
    return;
}

void newborn(background_jobs *background_job, int background_count)
{
    for(int i=0; i<background_count; i++)
    {
        printf("%d\n", background_job[i].id);
    }
    return;

}

void interrupt()
{
    FILE *f = fopen("/proc/interrupts", "r");
    if(f == NULL)
        printf("Can't open /proc/interrupts file\n");

    else
    {
        char *line = read_file("/proc/interrupts", 3);
        for(int i=0; i < strlen(line); i++)
        {
            if(line[i]=='I')
            {
                line[i] = '\0';
                break;
            }
        }
        printf("%s\n",line);
        free(line);  
    }

    fclose(f);
    return;
}

void handle_baywatch(char *input, background_jobs *background_job, int background_count)
{
    printf("%s\n",input);
    char copy[100];
    strcpy(copy,input);

    char **tokens_matrix = (char**)malloc(4*sizeof(char*));
    for(int i=0; i<4; i++)
    {
        tokens_matrix[i] = (char*)malloc(50*(sizeof(char)));
    }

    int i=0;
    char *token = strtok(copy, " \t");
    while(token != NULL)
    {
        strcpy(tokens_matrix[i],token);
        token = strtok(NULL, " \t");
        i++;
    }
    //printf("%s\n",tokens_matrix[0]);
    //printf("%s\n",tokens_matrix[1]);
    //printf("%s\n",tokens_matrix[2]);
    //printf("%d\n",i);

    baywatch(tokens_matrix,i,background_job,background_count);
    
}