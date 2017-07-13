#ifndef ENV_HEADER
#define ENV_HEADER

#define MASTER_PID_FILE     "./APP.pid"
#define MAX_WORKER_PROCESS  32

extern int CPU_NUM;
extern int g_worker_processes;

void env_init();
int bind_cpu(int cpuid);
void rlimit_init();
void set_proc_title(const char* name);
void proc_title_init(char** argv);
void create_pidfile(int pid);
int read_pidfile();
void delete_pidfile();

#endif

