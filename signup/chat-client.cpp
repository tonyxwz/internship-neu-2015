
#include "chat-client.h"
#include <QHostAddress>

ChatClient::ChatClient(QObject* parent /* = NULL */)
  : QTcpSocket(parent)
{
  connect(this, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
  connect(this, SIGNAL(connected()), this, SLOT(OnConnected()));
}

ChatClient::~ChatClient() {}

void
ChatClient::ConnectToServer(const char* ip, quint16 port)
{
  QString strip(ip);
  QHostAddress addr;
  addr.setAddress(strip);
  connectToHost(addr, port);
}

void
ChatClient::Close()
{
  disconnectFromHost();
  close();
}

bool
ChatClient::WriteData(const char* data, qint64 len)
{
  if (NULL == data || len <= 0)
    return false;
  qint64 ret = writeData(data, len);
  if (0 >= ret)
    return false;

  waitForBytesWritten(-1); // why ?
  printf("send    data: %s\n", data);

  waitForReadyRead(-1); // why ?

  return true;
}

void
ChatClient::ParseData(const char* data, qint64 len)
{
  qDebug() << data;
  // printf( "receive data: %s\n", data );
}

void
ChatClient::OnReadyRead()
{
  char data[BUFFERSIZE_MAX] = { 0 };
  qint64 len = readData(data, BUFFERSIZE_MAX);
  if (len <= 0)
    return;
  else
    ParseData(data, len);
}

void
ChatClient::OnConnected()
{
  printf("connected: %d !\n", socketDescriptor());
}
