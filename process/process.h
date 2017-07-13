#ifndef PROCESS_HEADER
#define PROCESS_HEADER

enum PROC_TYPE
{
    MASTER_PROCESS = 0,
    WORKER_PROCESS = 1
};

extern enum PROC_TYPE g_process_type;
extern int g_stop;
extern int g_exit;

void worker_run();
void master_run();
void on_worker_exit(int pid);
void process_init();

#endif

