#ifndef __BAYWATCH_H_
#define __BAYWATCH_H_


void dirty();
void interrupt();
void newborn(background_jobs *background_job, int background_count);
void handle_baywatch(char *input, background_jobs *background_job, int background_count);
void baywatch(char **args, int no_args, background_jobs *background_job, int background_count);
#endif