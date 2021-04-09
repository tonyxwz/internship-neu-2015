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
    void newConnect(); //���ӷ�����
    void readMessage();  //��������
    void displayError(QAbstractSocket::SocketError);  //��ʾ����
   // void mousePressEvent(QMouseEvent *);
   // void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_pushButton_clicked();

    void on_timenow_linkActivated(const QString &link);

private:
    QTcpSocket *tcpSocket;
    QString message;  //��Ŵӷ��������յ����ַ���
    quint16 blockSize;  //����ļ��Ĵ�С��Ϣ
    //QPoint windowPos;
   // QPoint mousePos;
   // QPoint dPos;


};

#endif // WIDGET_H
