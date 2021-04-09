#ifndef M_EPOLL
#define M_EPOLL
#include <sys/epoll.h>
#include <fcntl.h>
int
setnoblocking(int fd);
void
addfd(int epollfd, int fd);

#endif
