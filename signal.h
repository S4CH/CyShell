#ifndef __SIGNAL_H_
#define __SIGNAL_H_

void identifySignal(char *input, background_jobs *background_job);
void passSignal(int pid, int signal);


#endif