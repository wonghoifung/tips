#include <ev.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

ev_io stdin_watcher;
ev_timer timeout_watcher;
ev_signal signal_watcher;
ev_child child_watcher;
ev_stat stat_watcher;

static void stdin_cb(EV_P_ ev_io* w, int revents)
{
  puts("stdin ready");
  // one-shot event, stop watcher manually
  ev_io_stop(EV_A_ w);
  // all nested ev_run stop
  ev_break(EV_A_ EVBREAK_ALL);
}

static void timeout_cb(EV_P_ ev_timer* w, int revents)
{
  puts("timeout");
  // innermost ev_run stop
  // ev_break(EV_A_ EVBREAK_ONE);
}

static void sigint_cb(EV_P_ ev_signal* w, int revents)
{
  static int count=0;
  count += 1;
  if (count == 5) ev_break(EV_A_ EVBREAK_ALL);
  puts("sigint");
}

static void child_cb(EV_P_ ev_child* w, int revents)
{
  puts("child");
  ev_child_stop(EV_A_ w);
}

static void stat_cb(EV_P_ ev_stat* w, int revents)
{
  if (w->attr.st_nlink)
  {
    printf("current size %ld\n",(long)w->attr.st_size);
    printf("current atime %ld\n",(long)w->attr.st_atime);
    printf("current mtime %ld\n",(long)w->attr.st_mtime);
  }
  else
  {
    puts("file not found");
  }
}

int main()
{
  struct ev_loop* loop=EV_DEFAULT;

  ev_io_init(&stdin_watcher, stdin_cb, 0, EV_READ);
  ev_io_start(loop, &stdin_watcher);

  ev_timer_init(&timeout_watcher, timeout_cb, 0., 3.);
  ev_timer_again(loop, &timeout_watcher);

  ev_signal_init(&signal_watcher, sigint_cb, SIGINT);
  ev_signal_start(loop, &signal_watcher);
  
  pid_t pid = fork();
  if (pid < 0) 
  {
    puts("fork error");
    exit(-1);
  }
  else if (pid == 0)
  {
    puts("I am child");
    sleep(2);
    exit(1);
  }
  else
  {
    ev_child_init(&child_watcher, child_cb, pid, 0);
    ev_child_start(loop, &child_watcher);
  }

  ev_stat_init(&stat_watcher, stat_cb, "./main.cpp", 0);
  ev_stat_start(loop, &stat_watcher);

  ev_run(loop,0);
  return 0;
}

