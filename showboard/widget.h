#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QTcpSocket>
#include <QString>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

public slots:
    void timeCountsFunction();
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误
   // void mousePressEvent(QMouseEvent *);
   // void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_pushButton_clicked();

    void on_timenow_linkActivated(const QString &link);

private:
    QTcpSocket *tcpSocket;
    QString message;  //存放从服务器接收到的字符串
    quint16 blockSize;  //存放文件的大小信息
    //QPoint windowPos;
   // QPoint mousePos;
   // QPoint dPos;


};

#endif // WIDGET_H
