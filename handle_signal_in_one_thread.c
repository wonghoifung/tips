#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void sig_handler(int signum)
{
	static int j=0;
	static int k=0;
	pthread_t sig_ppid = pthread_self();
	if (signum==SIGUSR1)//10
	{
		printf("thread %d, receive SIGUSR1 No. %d\n",sig_ppid,j);
		j++;
	} 
	else if (signum==SIGRTMIN)//34
	{
		printf("thread %d, receive SIGRTMIN No. %d\n",sig_ppid,k);
		k++;
	}
}

void* worker_thread()
{
	pthread_t ppid=pthread_self();
	//pthread_detach(ppid);
	while (1)
	{
		printf("i am thread %d\n",ppid);
		sleep(10);
	}
}

void* sigmgr_thread()
{
	sigset_t waitset;
	siginfo_t info;
	int rc;
	pthread_t ppid = pthread_self();
	//pthread_detach(ppid);
	sigemptyset(&waitset);
	sigaddset(&waitset,SIGRTMIN);
	sigaddset(&waitset,SIGUSR1);
	while (1)
	{
		rc = sigwaitinfo(&waitset,&info);
		if (rc != -1)
		{
			printf("sigwaitinfo() fetch the signal:%d rc:%d\n",info.si_signo,rc);
			sig_handler(info.si_signo);
		}
		else
		{
			printf("sigwaitinfo() returned err:%d %s\n",errno,strerror(errno));
		}
	}
}

int main()
{
	sigset_t bset,oset;
	int i;
	pid_t pid=getpid();
	pthread_t ppid;
	sigemptyset(&bset);
	sigaddset(&bset,SIGRTMIN);
	sigaddset(&bset,SIGUSR1);
	if (pthread_sigmask(SIG_BLOCK,&bset,&oset)!=0)
	{
		printf("set pthread mask failed\n");
		return -1;
	}
	printf("pid:%d\n",pid);
	pthread_create(&ppid,NULL,sigmgr_thread,NULL);
	for (i=0;i<5;i++)
	{
		pthread_create(&ppid,NULL,worker_thread,NULL);
	}
	for (i=0; i<50; i++)
	{
		/*kill(pid,SIGUSR1);
		printf("main thread, send sigusr1 no. %d\n",i);
		kill(pid,SIGRTMIN);
		printf("main thread, send sigrtmin no. %d\n", i);*/
		sleep(10);
	}
	return 0;
}

