#ifndef __CHECK_H_
#define __CHECK_H_

typedef struct background_jobs background_jobs;

struct background_jobs
{
    char name[100];
    int id;
    int number;
};


void check_commands(char*input,background_jobs *background_job,int*background_count,char *present, int *fg_process_id);
void command(char*string, int background, background_jobs *background_job, int*background_counut, char *present, int *fg_process_id);


#endif