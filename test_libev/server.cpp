#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ev.h>
#define MAXLEN 1023
#define PORT 9999
#define ADDR_IP "127.0.0.1"

int socket_init();
void accept_callback(struct ev_loop* loop, ev_io* w, int revents);
void recv_callback(struct ev_loop* loop, ev_io* w, int revents);
void write_callback(struct ev_loop* loop, ev_io* w, int revents);

typedef struct conn_queue_item CQ_ITEM;
struct conn_queue_item
{
  int sfd;
  char szAddr[16];
  int port;
  CQ_ITEM* next;
};

typedef struct conn_queue CQ;
struct conn_queue
{
  CQ_ITEM* head;
  CQ_ITEM* tail;
  pthread_mutex_t lock;
};

static void cq_init(CQ* cq)
{
  pthread_mutex_init(&cq->lock, NULL);
  cq->head = NULL;
  cq->tail = NULL;
}

static CQ_ITEM* cq_pop(CQ* cq)
{
  CQ_ITEM* item;
  pthread_mutex_lock(&cq->lock);
  item = cq->head;
  if (NULL != item)
  {
    cq->head = item->next;
    if (NULL == cq->head) cq->tail = NULL;
  }
  pthread_mutex_unlock(&cq->lock);
  return item;
}

static void cq_push(CQ* cq, CQ_ITEM* item)
{
  item->next = NULL;
  pthread_mutex_lock(&cq->lock);
  if (NULL == cq->tail) cq->head = item;
  else cq->tail->next = item;
  cq->tail = item;
  pthread_mutex_unlock(&cq->lock);
}

typedef struct
{
  pthread_t thread_id;
  struct ev_loop* loop;
  struct ev_async async_watcher;
  struct conn_queue* new_conn_queue;
} WORK_THREAD;

typedef struct 
{
  pthread_t thread_id;
  struct ev_loop* loop;
  struct ev_io accept_watcher;
} DISPATCHER_THREAD;

static DISPATCHER_THREAD dispatcher_thread;
static WORK_THREAD* work_threads;
static int init_count = 0;
static pthread_mutex_t init_lock;
static pthread_cond_t init_cond;
static int round_robin = 0;

static void* worker_libev(void* arg)
{
  WORK_THREAD* me = (WORK_THREAD*)arg;
  // thread setup ...
  pthread_mutex_lock(&init_lock);
  init_count++;
  pthread_cond_signal(&init_cond);
  pthread_mutex_unlock(&init_lock);
  me->thread_id = pthread_self();
  ev_loop(me->loop, 0);
  return NULL;
}

static void create_worker(void* (*func)(void*), void* arg)
{
  pthread_t thread;
  pthread_attr_t attr;
  int ret;
  pthread_attr_init(&attr);
  if ((ret = pthread_create(&thread, &attr, func, arg)) != 0)
  {
    fprintf(stderr, "cannot create thread: %s\n", strerror(ret));
    exit(1);
  }
}

static void async_cb(EV_P_ ev_async* w, int revents)
{
  CQ_ITEM* item;
  item = cq_pop(((WORK_THREAD*)(w->data))->new_conn_queue);
  if (NULL != item)
  {
    ev_io* recv_watcher = (ev_io*)malloc(sizeof(ev_io));
    ev_io_init(recv_watcher, recv_callback, item->sfd, EV_READ);
    ev_io_start(((WORK_THREAD*)(w->data))->loop, recv_watcher);
    printf("thread[%lu] accept fd:%d addr:%s port:%d\n",
      ((WORK_THREAD*)(w->data))->thread_id, item->sfd, item->szAddr, item->port);
    free(item);
    item=NULL;
  }
}

static void setup_thread(WORK_THREAD* me)
{
  me->loop = ev_loop_new(0);
  if (!me->loop)
  {
    fprintf(stderr, "cannot allocate event base\n");
    exit(1);
  }
  me->async_watcher.data = me;
  ev_async_init(&me->async_watcher, async_cb);
  ev_async_start(me->loop, &me->async_watcher);
  me->new_conn_queue = (CQ*)malloc(sizeof(struct conn_queue));
  if (me->new_conn_queue == NULL)
  {
    perror("failed to allocate memory for connection queue\n");
    exit(EXIT_FAILURE);
  }
  cq_init(me->new_conn_queue);
}

void thread_init()
{
  int nthreads = 4;
  pthread_mutex_init(&init_lock, NULL);
  pthread_cond_init(&init_cond, NULL);
  work_threads = (WORK_THREAD*)calloc(nthreads, sizeof(WORK_THREAD));
  if (!work_threads)
  {
    perror("cannot allocate thread descriptors\n");
    exit(1);
  }
  dispatcher_thread.loop = ev_default_loop(0); // why not EV_DEFAULT?
  dispatcher_thread.thread_id = pthread_self(); 
  int i=0;
  for (i=0; i<nthreads; i++) setup_thread(&work_threads[i]);
  for (i=0; i<nthreads; i++) create_worker(worker_libev, &work_threads[i]);
  pthread_mutex_lock(&init_lock);
  while (init_count < nthreads) pthread_cond_wait(&init_cond, &init_lock);
  pthread_mutex_unlock(&init_lock);
}

int main(int argc, char** argv)
{
  thread_init();
  int listen = socket_init();
  ev_io_init(&(dispatcher_thread.accept_watcher), accept_callback, listen, EV_READ);
  ev_io_start(dispatcher_thread.loop, &(dispatcher_thread.accept_watcher));
  ev_loop(dispatcher_thread.loop, 0);
  ev_loop_destroy(dispatcher_thread.loop);
  return 0;
}

int socket_init()
{
  struct sockaddr_in my_addr;
  int listener;
  if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket");
    exit(1);
  }
  else 
  {
    printf("socket create success\n");
  }
  int so_reuseaddr = 1;
  setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));
  bzero(&my_addr, sizeof(my_addr));
  my_addr.sin_family = PF_INET;
  my_addr.sin_port = htons(PORT);
  my_addr.sin_addr.s_addr = inet_addr(ADDR_IP);
  if (bind(listener, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
  {
    perror("bind error\n");
    exit(1);
  }
  else
  {
    printf("ip bind success: %s\n", ADDR_IP);
  }
  if (listen(listener, 1024) == -1)
  {
    perror("listen error\n");
    exit(1);
  }
  else
  {
    printf("listen success, port: %d\n", PORT);
  }
  return listener;
}

void dispatch_conn(int anewfd, struct sockaddr_in asin)
{
  CQ_ITEM* ipNewItem = (CQ_ITEM*)calloc(1,sizeof(CQ_ITEM));
  if (!ipNewItem)
  {
    perror("cannot allocate connection item\n");
    exit(1);
  }
  ipNewItem->sfd = anewfd;
  strcpy(ipNewItem->szAddr, (char*)inet_ntoa(asin.sin_addr));
  ipNewItem->port = asin.sin_port;
  int robin = round_robin % init_count;
  cq_push(work_threads[robin].new_conn_queue, ipNewItem);
  ev_async_send(work_threads[robin].loop, &(work_threads[robin].async_watcher));
  round_robin++;
}

void accept_callback(struct ev_loop* loop, ev_io* w, int revents)
{
  int newfd;
  struct sockaddr_in sin;
  socklen_t addrlen = sizeof(struct sockaddr);
  while ((newfd = accept(w->fd, (struct sockaddr*)&sin, &addrlen)) < 0)
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
    {
      continue;
    }
    else
    {
      printf("accept error: %s\n", strerror(errno));
      break;
    }
  }
  dispatch_conn(newfd, sin);
}

void recv_callback(struct ev_loop* loop, ev_io* w, int revents)
{
  char buffer[1024] = {0};
  int ret = 0;
  loop: 
    ret = recv(w->fd, buffer, MAXLEN, 0);
    if (ret > 0)
    {
      printf("thread[%lu] recv message: %s\n",pthread_self(),buffer);
    }
    else if (ret == 0)
    {
      printf("thread[%lu] remote socket closed! fd: %d\n",pthread_self(),w->fd);
      close(w->fd);
      ev_io_stop(loop, w);
      free(w);
      return;
    }
    else
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
      {
        goto loop;
      }
      else
      {
        printf("thread[%lu] ret: %d, close fd: %d\n",pthread_self(),ret,w->fd);
        close(w->fd);
        ev_io_stop(loop, w);
        free(w);
        return;
      }
    }
    int fd = w->fd;
    ev_io_stop(loop, w);
    ev_io_init(w,write_callback,fd,EV_WRITE);
    ev_io_start(loop, w);
    printf("thread[%lu] fd: %d, turn read to write loop\n",pthread_self(),fd);
}

void write_callback(struct ev_loop* loop, ev_io* w, int revents)
{
  char buffer[1024]={0};
  snprintf(buffer,1023,"hello world!");
  write(w->fd, buffer, strlen(buffer));
  int fd = w->fd;
  ev_io_stop(loop, w);
  ev_io_init(w, recv_callback, fd, EV_READ);
  ev_io_start(loop, w);
}

