#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "signalop.h"
#include "process.h"
#include "env.h"

#define INVALID_PID -1

struct process
{
    pid_t pid;
    int cpuid;
};

static struct process g_process[MAX_WORKER_PROCESS];
static int g_master_pid;
static int g_process_pid;
enum PROC_TYPE g_process_type;

static int g_all_workers_exit = 0;
static int g_create_worker = 1;

int g_stop = 0;
int g_exit = 0;

static int worker_empty()
{
    int i;
    for (i = 0; i < g_worker_processes; i++)
    {
        struct process* p = &g_process[i];
        if (p->pid != INVALID_PID)
            return 0;
    }
    return 1;
}

static pid_t fork_worker(struct process* p)
{
    pid_t pid = fork();

    switch (pid)
    {
        case -1:
            printf("Failed to fork worker process. master pid:%d\n", getpid());
            return -1;

        case 0:
            g_process_pid = getpid();
            g_process_type = WORKER_PROCESS;
            set_proc_title("app: worker process");

            if (bind_cpu(p->cpuid))
            {
                printf("Failed to bind cpu: %d\n", p->cpuid);
                exit(0);
            }

            return 0;

        default:
            p->pid = pid;
            return pid;
    }
}

static void spawn_worker_process()
{
    int i;

    for (i = 0; i < g_worker_processes; i++)
    {
        struct process *p = &g_process[i];
        if (p->pid != INVALID_PID)
            continue;

        if (0 == fork_worker(p))
            break;
    }
}

void worker_run()
{
	while (true)
	{
		if (g_stop)
		{
			printf("worker stop\n");
			break;
		}

		if (g_exit) 
		{
			printf("worker exit\n");
			exit(0);
		}

		// printf("working...\n");
		sleep(1);
	}
}

static void send_signal_to_workers(int signo)
{
    int i;

    for (i = 0; i < g_worker_processes; i++)
    {
        struct process *p = &g_process[i];
        if (p->pid != INVALID_PID)
        {
            if (kill(p->pid, signo) == -1)
                printf("Failed to send signal %d to child pid:%d\n", signo, p->pid);
        }
    }
}

void master_run()
{

    for (;;)
    {
        if (g_stop == 1)
        {
            printf("notify worker processes to stop\n");
            send_signal_to_workers(SIGQUIT);
            g_stop = 2;
        }

        if (g_exit == 1)
        {
            printf("notify worker processes to direct exit\n");
            send_signal_to_workers(SIGINT);
            g_exit = 2;
        }

        if (g_all_workers_exit == 1)
        {
            printf("app exit now...\n");
            delete_pidfile();
            exit(0);
        }

        if (g_create_worker)
        {
            g_create_worker = 0;
            printf("master spawn worker\n");
            spawn_worker_process();
            if (g_process_pid != g_master_pid)
                break;
        }

        usleep(10000);
    }
}

void on_worker_exit(int pid)
{
    int i;

    for (i = 0; i < g_worker_processes; i++)
    {
        struct process *p = &g_process[i];
        if (p->pid == pid)
            p->pid = INVALID_PID;
    }

    if (!g_stop && !g_exit)
    {
    	printf("set g_create_worker = 1 to respawn worker\n");
        g_create_worker = 1;
    }

    if (worker_empty() && (g_stop || g_exit))
        g_all_workers_exit = 1;
}

void process_init()
{
    int i;

    g_process_pid = getpid();
    g_master_pid = g_process_pid;
    g_process_type = MASTER_PROCESS;
    set_proc_title("app: master process");
    create_pidfile(g_master_pid);

    for (i = 0; i < g_worker_processes; i++)
    {
        struct process *p = &g_process[i];
        p->pid = INVALID_PID;
        p->cpuid = i % CPU_NUM;
    }
}

