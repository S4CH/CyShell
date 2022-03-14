#ifndef __FG_H_
#define __FG_H_

bool handle_fg(background_jobs *background_job, char *input, int *fg_process_id);
bool fg(int pid, char*process_name, int*fg_process_id);


#endif