#include "msocket.h"
//#include "common.h"
#include "mepoll.h"
#include "threadpool.h"

#define POOL_NUM 3

int
main()
{
  int fp[4], *new_sock;
  char* message;
  /*****************************initbase************************************/
  sqlite3* db = NULL;
  if (initsqbase("mybase.db", &db) == 0) {
    puts("You have open db successfully!\n");
  };

  // printf("%s",keshi);
  // illness a={"new",1,"2","2","13312241223","wdawd",3};
  // sqbaseinsert(&db,"illness",a);
  // int row=0,col=0;
  // char **result;
  // sqbasesearch(&db,&row,&col,&result,"illness",NULL);
  // sqbasechart(&db);
  /*************************data struct************************************/
  /*
  for(int i=0;i<(row + 1) * col;i++){
  if((i-1)%9==0&&i>9){
  strcat(b[(i-1)/9-1].name,result[i]);
  printf ("result[%d] = %s\n", i, result[i]);}
  }
  /********************************datastruct***********************************/
  // sqlite3_free_table (result);

  /***********************************initbase******************************/

  /******************************init
   * threadpool*********************************/
  pool_init(POOL_NUM);

  /***************************************************************/

  struct sockaddr_in client;
  if (m_create_socket(&fp[0], 1234) != 0) {
    puts("error:1 getnum");
    exit(1);
  }
  if (m_create_socket(&fp[1], 2345) != 0) {
    puts("error:2 nurse");
    exit(1);
  }
  if (m_create_socket(&fp[2], 3456) != 0) {
    puts("error:3 doctor");
    exit(1);
  }
  if (m_create_socket(&fp[3], 4567) != 0) {
    puts("error:4 showboard");
    exit(1);
  }
  listen(fp[0], 5);
  listen(fp[1], 5);
  listen(fp[2], 5);
  listen(fp[3], 5);
  struct epoll_event event[5];
  int epollfd = epoll_create(5);
  addfd(epollfd, fp[0]);
  addfd(epollfd, fp[1]);
  addfd(epollfd, fp[2]);
  addfd(epollfd, fp[3]);
  puts("Waiting for incoming connections...");
  while (1) {
    int num = epoll_wait(epollfd, event, 5, -1);
    assert(num != -1);
    if (0 == num) {
      continue;
    }
    for (int i = 0; i < num; i++) {
      if (fp[0] == event[i].data.fd) {
        puts("first events");
        int c = sizeof(struct sockaddr_in);
        int new_socket =
          accept(fp[0], (struct sockaddr*)&client, (socklen_t*)&c);
        if (new_socket < 0) {
          perror("accept failed");
        }
        // Reply to the client
        // message = "Hello Client , I have received your connection. But I have
        // to go now, bye \n"; write(new_socket , message , strlen(message));
        puts("Connection accepted");
        new_sock = malloc(1);
        *new_sock = new_socket;
        pool_add_worker(myprocess_1234, new_sock, &db);

      } else if (fp[3] == event[i].data.fd) {
        puts("third events");
        int c = sizeof(struct sockaddr_in);
        int new_socket =
          accept(fp[3], (struct sockaddr*)&client, (socklen_t*)&c);
        if (new_socket < 0) {
          perror("accept failed");
        }

        // message = "Hello Client , I have received your connection. But I have
        // to go now, bye \n"; write(new_socket , message , strlen(message));
        pthread_t* sniffer_thread;
        // new_sock = malloc(1);
        //*new_sock = new_socket;
        passarg* new_sock = malloc(sizeof(passarg));
        new_sock->i = new_socket;
        new_sock->db = &db;
        if (pthread_create(sniffer_thread, NULL, cooncetevent_4567, new_sock)) {
          puts("");
        }
        // close(new_socket);

      } else if (fp[2] == event[i].data.fd) {
        puts("first events");
        int c = sizeof(struct sockaddr_in);
        int new_socket =
          accept(fp[2], (struct sockaddr*)&client, (socklen_t*)&c);
        if (new_socket < 0) {
          perror("accept failed");
        }
        // Reply to the client
        // message = "Hello Client , I have received your connection. But I have
        // to go now, bye \n"; write(new_socket , message , strlen(message));
        puts("3456_Connection accepted");
        new_sock = malloc(1);
        *new_sock = new_socket;
        pool_add_worker(myprocess_3456, new_sock, &db);

      }

      else {
        puts("second events");
        int c = sizeof(struct sockaddr_in);
        int new_socket =
          accept(fp[1], (struct sockaddr*)&client, (socklen_t*)&c);
        if (new_socket < 0) {
          perror("accept failed");
        }
        // message = "Hello Client2 , I have received your connection. But I
        // have to go now, bye\n"; write(new_socket , message , strlen(message));
        puts("Connection accepted");
        new_sock = malloc(1);
        *new_sock = new_socket;
        pool_add_worker(myprocess, new_sock, &db);
      }
    }
  }
  return 0;
}
