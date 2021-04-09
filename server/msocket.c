#include "msocket.h"
int
m_create_socket(int* fp, int iport)
{
  struct sockaddr_in server;
  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    return 2;
  }
  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(iport);
  // Bind
  if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {
    return 1;
  }
  *fp = socket_desc;
  return 0;
}
