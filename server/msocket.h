#ifndef M_SOCKET
#define M_SOCKET
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
int
m_create_socket(int* fp, int iport);

#endif
