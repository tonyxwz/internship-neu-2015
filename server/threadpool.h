#ifndef THREADPOOL
#define THREADPOOL
#include "cJSON.h"
#include "msqbase.h"
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct worker
{
  /*回调函数，任务运行时会调用此函数，注意也可声明成其它形式*/
  void* (*process)(void* arg, sqlite3** arg1);
  void* arg; /*回调函数的参数*/
  sqlite3** arg1;
  struct worker* next;

} CThread_worker;

/*线程池结构*/
typedef struct
{
  pthread_mutex_t queue_lock;
  pthread_cond_t queue_ready;

  /*链表结构，线程池中所有等待任务*/
  CThread_worker* queue_head;

  /*是否销毁线程池*/
  int shutdown;
  pthread_t* threadid;
  /*线程池中允许的活动线程数目*/
  int max_thread_num;
  /*当前等待队列的任务数目*/
  int cur_queue_size;

} CThread_pool;

static CThread_pool* pool = NULL;
pthread_mutex_t basemutex;
sem_t sem_n;

int
pool_add_worker(void* (*process)(void* arg, sqlite3** arg1),
                void* arg,
                sqlite3** arg1);
void*
thread_routine(void* arg);
void
pool_init(int max_thread_num);
int
pool_destroy();
void*
myprocess(void* arg, sqlite3** arg1);
void*
myprocess_1234(void* arg, sqlite3** arg1);
void*
cooncetevent_4567(void* arg);
void*
myprocess_3456(void* arg, sqlite3** arg1);

#endif
