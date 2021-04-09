#include "threadpool.h"
#define database_col 10
/*
 *线程池里所有运行和等待的任务都是一个CThread_worker
 *由于所有任务都在链表里，所以是一个链表结构
 */

// share resource

void
pool_init(int max_thread_num)
{
  pool = (CThread_pool*)malloc(sizeof(CThread_pool));
  pthread_mutex_init(&(pool->queue_lock), NULL);
  pthread_cond_init(&(pool->queue_ready), NULL);
  pthread_mutex_init(&basemutex, NULL); // new
  sem_init(&sem_n, 0, 0);
  pool->queue_head = NULL;

  pool->max_thread_num = max_thread_num;
  pool->cur_queue_size = 0;

  pool->shutdown = 0;

  pool->threadid = (pthread_t*)malloc(max_thread_num * sizeof(pthread_t));
  int i = 0;
  for (i = 0; i < max_thread_num; i++) {
    pthread_create(&(pool->threadid[i]), NULL, thread_routine, NULL);
  }
}

/*向线程池中加入任务*/
int
pool_add_worker(void* (*process)(void* arg, sqlite3** arg1),
                void* arg,
                sqlite3** arg1)
{
  /*构造一个新任务*/
  CThread_worker* newworker = (CThread_worker*)malloc(sizeof(CThread_worker));
  newworker->process = process;
  newworker->arg = arg;
  newworker->arg1 = arg1;
  newworker->next = NULL; /*别忘置空*/

  pthread_mutex_lock(&(pool->queue_lock));
  /*将任务加入到等待队列中*/
  CThread_worker* member = pool->queue_head;
  if (member != NULL) {
    while (member->next != NULL)
      member = member->next;
    member->next = newworker;
  } else {
    pool->queue_head = newworker;
  }

  assert(pool->queue_head != NULL);

  pool->cur_queue_size++;
  pthread_mutex_unlock(&(pool->queue_lock));
  /*好了，等待队列中有任务了，唤醒一个等待线程；
  注意如果所有线程都在忙碌，这句没有任何作用*/
  pthread_cond_signal(&(pool->queue_ready));
  return 0;
}

/*销毁线程池，等待队列中的任务不会再被执行，但是正在运行的线程会一直
把任务运行完后再退出*/
int
pool_destroy()
{
  if (pool->shutdown)
    return -1; /*防止两次调用*/
  pool->shutdown = 1;

  /*唤醒所有等待线程，线程池要销毁了*/
  pthread_cond_broadcast(&(pool->queue_ready));

  /*阻塞等待线程退出，否则就成僵尸了*/
  int i;
  for (i = 0; i < pool->max_thread_num; i++)
    pthread_join(pool->threadid[i], NULL);
  free(pool->threadid);

  /*销毁等待队列*/
  CThread_worker* head = NULL;
  while (pool->queue_head != NULL) {
    head = pool->queue_head;
    pool->queue_head = pool->queue_head->next;
    free(head);
  }
  /*条件变量和互斥量也别忘了销毁*/
  pthread_mutex_destroy(&(pool->queue_lock));
  pthread_cond_destroy(&(pool->queue_ready));

  free(pool);
  /*销毁后指针置空是个好习惯*/
  pool = NULL;
  return 0;
}

void*
thread_routine(void* arg)
{
  printf("starting thread 0x%x\n", pthread_self());
  while (1) {
    pthread_mutex_lock(&(pool->queue_lock));
    /*如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意
    pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁*/
    while (pool->cur_queue_size == 0 && !pool->shutdown) {
      printf("thread 0x%x is waiting\n", pthread_self());
      pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
    }

    /*线程池要销毁了*/
    if (pool->shutdown) {
      /*遇到break,continue,return等跳转语句，千万不要忘记先解锁*/
      pthread_mutex_unlock(&(pool->queue_lock));
      printf("thread 0x%x will exit\n", pthread_self());
      pthread_exit(NULL);
    }

    printf("thread 0x%x is starting to work\n", pthread_self());

    /*assert是调试的好帮手*/
    assert(pool->cur_queue_size != 0);
    assert(pool->queue_head != NULL);
    /*等待队列长度减去1，并取出链表中的头元素*/
    pool->cur_queue_size--;
    CThread_worker* worker = pool->queue_head;
    pool->queue_head = worker->next;
    pthread_mutex_unlock(&(pool->queue_lock));

    /*调用回调函数，执行任务*/
    (*(worker->process))(worker->arg, worker->arg1);
    free(worker);
    worker = NULL;
  }
  pthread_exit(NULL);
}

void*
myprocess(void* arg, sqlite3** arg1)
{

  int sock = *(int*)arg;
  sqlite3** db = arg1;
  int read_size;
  cJSON *json = NULL, *json_action = NULL, *json_username = NULL,
        *json_password = NULL, *json_id = NULL;
  char *message, client_message[2000];
  memset(client_message, 0, 2000);
  // Send some messages to the client
  read_size = read(sock, client_message, 2000);
  if (read_size == 0) {
    puts("Client disconnected");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }
  printf("%s\n", client_message);
  // char*
  // client_message1="{\"action\":\"1\",\"username\":\"www\",\"password\":\"www\"}";
  json = cJSON_Parse(client_message);
  if (!json) {
    printf("Error before: [%s]\n", cJSON_GetErrorPtr());
  } else {
    json_action = cJSON_GetObjectItem(json, "action");
    if (!strcmp(json_action->valuestring, "1")) {
      json_username = cJSON_GetObjectItem(json, "username");
      json_password = cJSON_GetObjectItem(json, "password");
      int stat = loginin(
        &db, "nurse", json_username->valuestring, json_password->valuestring);
      if (stat == 1) {
        message = "1\n";
      } else {
        message = "0\n";
      }
      write(sock, message, strlen(message));
    } else if (!strcmp(json_action->valuestring, "3")) {
      json_id = cJSON_GetObjectItem(json, "id");
      json_password = cJSON_GetObjectItem(json, "doctorname");
      json_username = cJSON_GetObjectItem(json, "username");
      int ts =
        selectdoctor(db, json_id->valuestring, json_password->valuestring);

      char** result;
      int row, col;
      char keshi[20] = { 0 };
      pthread_mutex_lock(&basemutex); // new
      getnursekeshi(db, keshi, json_username->valuestring);
      sqbasesearch(db, &row, &col, &result, "illness", keshi);
      pthread_mutex_unlock(&basemutex); // new
      cJSON* root = cJSON_CreateObject();
      cJSON_AddNumberToObject(root, "num", row);
      for (int i = 0; i < (row + 1) * col; i++) {
        if ((i % database_col == 0) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "id%d", i / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 1) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "name%d", (i - 1) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 2) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "sex%d", (i - 2) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 3) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "age%d", (i - 3) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 9) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "time%d", (i - 9) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
      }
      char* out;
      out = cJSON_Print(root);
      cJSON_Delete(root);
      printf("%s\n", out);
      write(sock, out, strlen(out));
      free(out);
      sqlite3_free_table(result);

    } else {
      json_username = cJSON_GetObjectItem(json, "username");
      char** result;
      int row, col;
      char keshi[20] = { 0 };
      pthread_mutex_lock(&basemutex); // new
      getnursekeshi(db, keshi, json_username->valuestring);
      sqbasesearch(db, &row, &col, &result, "illness", keshi);
      pthread_mutex_unlock(&basemutex); // new
      cJSON* root = cJSON_CreateObject();
      cJSON_AddNumberToObject(root, "num", row);
      for (int i = 0; i < (row + 1) * col; i++) {
        if ((i % database_col == 0) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "id%d", i / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 1) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "name%d", (i - 1) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 2) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "sex%d", (i - 2) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 3) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "age%d", (i - 3) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 9) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "time%d", (i - 9) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
      }
      char* out;
      out = cJSON_Print(root);
      cJSON_Delete(root);
      printf("%s\n", out);
      write(sock, out, strlen(out));
      free(out);
      sqlite3_free_table(result);
    }
  }
  close(sock);
  free(arg);
  cJSON_Delete(json);
  return NULL;
}

void*
myprocess_1234(void* arg, sqlite3** arg1)
{

  int sock = *(int*)arg;
  sqlite3** db = arg1;
  int read_size;
  cJSON *json = NULL, *json_le1 = NULL, *json_le2 = NULL, *json_le3 = NULL,
        *json_le4 = NULL, *json_le5 = NULL, *json_ls5 = NULL;
  char *message, client_message[2000];
  memset(client_message, 0, 2000);
  read_size = read(sock, client_message, 2000);
  if (read_size == 0) {
    puts("Client disconnected");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }
  printf("%s\n", client_message);
  json = cJSON_Parse(client_message);
  if (!json) {
    printf("Error before: [%s]\n", cJSON_GetErrorPtr());
  } else {
    json_le1 = cJSON_GetObjectItem(json, "le1");

    json_le2 = cJSON_GetObjectItem(json, "le2");

    json_le3 = cJSON_GetObjectItem(json, "le3");

    json_le4 = cJSON_GetObjectItem(json, "le4");
    json_le5 = cJSON_GetObjectItem(json, "le5");
    json_ls5 = cJSON_GetObjectItem(json, "ls5");
    illness* person = (illness*)malloc(sizeof(illness));
    memset(person, 0, sizeof(illness));
    strcat(person->name, json_le1->valuestring);
    strcat(person->sex, json_le2->valuestring);
    strcat(person->age, json_le3->valuestring);
    strcat(person->telephone, json_le4->valuestring);
    strcat(person->department, json_ls5->valuestring);
    strcat(person->ybid, json_le5->valuestring);
    strcat(person->doctorname, "");
    strcat(person->status, "0");
    pthread_mutex_lock(&basemutex);
    sqbaseinsert(db, "illness", *person);
    pthread_mutex_unlock(&basemutex);
    int row;
    getnumber(db, person->department, &row);
    printf("your num %d", row);
    char strnum[6] = { 0 };
    sprintf(strnum, "%d", row);
    write(sock, strnum, strlen(strnum));
  }

  close(sock);
  free(arg);
  cJSON_Delete(json);
  return NULL;
}

void*
cooncetevent_4567(void* arg)
{
  int sock = ((passarg*)arg)->i;
  sqlite3** db = ((passarg*)arg)->db;
  int read_size;
  char *message, client_message[3];
  static int i = 0;
  int row;
  getdnumber(db, "1", &row);
  // printf("the num is %d\n",row);
  memset(client_message, 0, 3);
  sprintf(client_message, "%d", row + 1);
  write(sock, client_message, strlen(client_message));
  while (1) {
    sem_wait(&sem_n); // new
    getdnumber(db, "1", &row);
    // printf("the num is %d\n",row);
    memset(client_message, 0, 3);
    sprintf(client_message, "%d", row + 1);
    write(sock, client_message, strlen(client_message));
  }
  close(sock);
  free(arg);
}

void*
myprocess_3456(void* arg, sqlite3** arg1)
{
  int sock = *(int*)arg;
  sqlite3** db = arg1;
  int read_size;
  cJSON *json = NULL, *json_action = NULL, *json_username = NULL,
        *json_password = NULL, *json_id = NULL;
  char *message, client_message[2000];
  memset(client_message, 0, 2000);
  // Send some messages to the client
  read_size = read(sock, client_message, 2000);
  if (read_size == 0) {
    puts("Client disconnected");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }
  printf("%s\n", client_message);
  json = cJSON_Parse(client_message);
  if (!json) {
    printf("Error before: [%s]\n", cJSON_GetErrorPtr());
  } else {
    json_action = cJSON_GetObjectItem(json, "action");
    if (!strcmp(json_action->valuestring, "1")) {
      json_password = cJSON_GetObjectItem(json, "password");
      json_username = cJSON_GetObjectItem(json, "username");
      int stat = loginin(
        &db, "doctor", json_username->valuestring, json_password->valuestring);
      if (stat == 1) {
        message = "1\n";
      } else {
        message = "0\n";
      }
      write(sock, message, strlen(message));
    } else if (!strcmp(json_action->valuestring, "3")) {
      json_id = cJSON_GetObjectItem(json, "id");
      json_username = cJSON_GetObjectItem(json, "username");
      finishdoctor(db, json_id->valuestring);

      sem_post(&sem_n);

      char** result;
      int row, col;
      char keshi[20] = { 0 };
      pthread_mutex_lock(&basemutex); // new
      sqbasesearchbydoctor(
        db, &row, &col, &result, "illness", json_username->valuestring);
      pthread_mutex_unlock(&basemutex); // new
      cJSON* root = cJSON_CreateObject();
      cJSON_AddNumberToObject(root, "num", row);
      for (int i = 0; i < (row + 1) * col; i++) {
        if ((i % database_col == 0) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "id%d", i / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 1) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "name%d", (i - 1) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 2) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "sex%d", (i - 2) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 3) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "age%d", (i - 3) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 9) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "time%d", (i - 9) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
      }
      char* out;
      out = cJSON_Print(root);
      cJSON_Delete(root);
      printf("%s\n", out);
      write(sock, out, strlen(out));
      free(out);
      sqlite3_free_table(result);

    } else {
      json_username = cJSON_GetObjectItem(json, "username");
      char** result;
      int row, col;
      char keshi[20] = { 0 };
      pthread_mutex_lock(&basemutex); // new
      sqbasesearchbydoctor(
        db, &row, &col, &result, "illness", json_username->valuestring);
      pthread_mutex_unlock(&basemutex); // new
      cJSON* root = cJSON_CreateObject();
      cJSON_AddNumberToObject(root, "num", row);
      for (int i = 0; i < (row + 1) * col; i++) {
        if ((i % database_col == 0) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "id%d", i / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 1) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "name%d", (i - 1) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 2) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "sex%d", (i - 2) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 3) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "age%d", (i - 3) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
        if ((i % database_col == 9) && i >= database_col) {
          char keshi[20] = { 0 };
          sprintf(keshi, "time%d", (i - 9) / database_col);
          cJSON_AddStringToObject(root, keshi, result[i]);
        }
      }
      char* out;
      out = cJSON_Print(root);
      cJSON_Delete(root);
      printf("%s\n", out);
      write(sock, out, strlen(out));
      free(out);
      sqlite3_free_table(result);
    }
  }
  close(sock);
  free(arg);
  cJSON_Delete(json);
  return NULL;
}
