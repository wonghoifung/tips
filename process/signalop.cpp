#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "env.h"
#include "process.h"
#include "signalop.h"

struct signalop
{
    int signo;
    const char* signame;
    const char* name;
    void (*handler)(int signo);
};

static void signal_handler(int signo);

struct signalop g_signals[] =
{
    {SIGQUIT,   "SIGQUIT",      	"stop&quit",    signal_handler},
    {SIGTERM,   "SIGTERM",          "direct-quit",  signal_handler},
    {SIGINT,    "SIGINT",           "direct-quit",  signal_handler},
    {SIGHUP,    "SIGHUP",       	"reload",   	signal_handler},
    {SIGUSR1,   "SIGUSR1",      	"reopen",   	signal_handler},
    {SIGUSR2,   "SIGUSR2",      	"change-bin",	signal_handler},
    {SIGCHLD,   "SIGCHLD",          "",         	signal_handler},
    {SIGSYS,    "SIGSYS,ignore",    "", 			SIG_IGN},
    {SIGPIPE,   "SIGPIPE,ignore", 	"", 			SIG_IGN},
    {0, 		NULL,   			"", 			NULL}
};

static void sweep_workers()
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("worker process %d exited\n", pid);
        on_worker_exit(pid);
    }
}

static void master_signal_handler(int signo)
{
    switch (signo)
    {
        case SIGQUIT:
            g_stop = 1;
            break;
        case SIGTERM:
        case SIGINT:
            g_exit = 1;
            break;
        case SIGHUP:
        case SIGUSR1:
        case SIGUSR2:
            break;

        case SIGCHLD:
            sweep_workers();
            break;
    }
}

static void worker_signal_handler(int signo)
{
    switch (signo)
    {
        case SIGQUIT:
            g_stop = 1;
            break;
        case SIGTERM:
        case SIGINT:
            g_exit = 1;
            break;
    }
}

static void signal_handler(int signo)
{
    struct signalop *sig;

    for (sig = g_signals; sig->signo != 0; sig++)
    {
        if (sig->signo == signo)
            break;
    }

    switch (g_process_type)
    {
        case MASTER_PROCESS:
            master_signal_handler(signo);
            break;

        case WORKER_PROCESS:
            worker_signal_handler(signo);
            break;

        default:
            break;
    }
}

void signalop_init()
{
    struct signalop *sig;
    struct sigaction sa;

    for (sig = g_signals; sig->signo != 0; sig++)
    {
        memset(&sa, 0, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1)
        {
            printf("sigaction(%s) failed\n", sig->signame);
            exit(0);
        }
    }
}

