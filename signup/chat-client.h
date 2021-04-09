#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>

class ChatClient : public QTcpSocket
{
  Q_OBJECT

public:
  ChatClient(QObject* parent = NULL);
  ~ChatClient();

  void ConnectToServer(const char* ip, quint16 port);
  void Close();
  bool WriteData(const char* data, qint64 len);
  void OnReadyRead();

protected:
  void ParseData(const char* data, qint64 len);

private slots:
  void OnConnected();

private:
  enum
  {
    BUFFERSIZE_MAX = 1024
  };
};

#endif // CHATCLIENT_H
