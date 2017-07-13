#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include "env.h"

int CPU_NUM;
int g_worker_processes;
static char **app_argv;
extern char **environ;

static void set_worker_env()
{
    g_worker_processes = CPU_NUM > MAX_WORKER_PROCESS ? MAX_WORKER_PROCESS : CPU_NUM;
    printf("worker process num: %d\n",g_worker_processes);
}

void env_init()
{
    CPU_NUM = sysconf(_SC_NPROCESSORS_CONF);
    set_worker_env();
}

int bind_cpu(int cpuid)
{
    cpu_set_t mask;

    CPU_ZERO(&mask);
    CPU_SET(cpuid, &mask);

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
        return -1;

    return 0;
}

void rlimit_init()
{
    struct rlimit rlim, rlim_new;

    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = 100000;
        if (setrlimit(RLIMIT_NOFILE, &rlim_new) != 0)
        {
            printf("Failed to set rlimit file. exit!\n");
            exit(0);
        }
    }

    if (getrlimit(RLIMIT_CORE, &rlim) == 0)
    {
        rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
        if (setrlimit(RLIMIT_CORE, &rlim_new) != 0)
        {
            printf("Failed to set rlimit core. exit!\n");
            exit(0);
        }
    }
}

void set_proc_title(const char* name)
{
    strcpy(app_argv[0], name);
}

void proc_title_init(char** argv)
{
    int i;
    size_t len = 0;
    void *p;

    app_argv = argv;

    for (i = 1; argv[i]; i++)
        len += strlen(argv[i]) + 1;

    for (i = 0; environ[i]; i++)
        len += strlen(environ[i]) + 1;

    p = malloc(len);
    if (!p)
    {
        printf("Failed to alloc environ mem\n");
        exit(0);
    }

    memcpy(p, argv[1] ? argv[1] : environ[0], len);

    len = 0;
    for (i = 1; argv[i]; i++)
    {
        argv[i] = (char*)p + len;
        len += strlen(argv[i]) + 1;
    }

    for (i = 0; environ[i]; i++)
    {
        environ[i] = (char*)p + len;
        len += strlen(environ[i]) + 1;
    }
}

void create_pidfile(int pid)
{
    FILE *fp = fopen(MASTER_PID_FILE, "w+");
    if (!fp)
    {
        printf("%s %s\n", strerror(errno), MASTER_PID_FILE);
        exit(0);
    }

    fprintf(fp, "%d", pid);
    fclose(fp);
}

int read_pidfile()
{
    char buff[32];
    FILE *fp = fopen(MASTER_PID_FILE, "r");
    if (!fp)
    {
        printf("%s -> %s\n", strerror(errno), MASTER_PID_FILE);
        exit(0);
    }

    if (NULL == fgets(buff, sizeof(buff) - 1, fp))
    {
        fclose(fp);
        exit(0);
    }

    fclose(fp);
    return atoi(buff);
}

void delete_pidfile()
{
    struct stat statfile;

    if (stat(MASTER_PID_FILE, &statfile) == 0)
        remove(MASTER_PID_FILE);
}
