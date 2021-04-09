#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QTcpSocket>
#include <stdio.h>
#include <stdlib.h>
#include "sethostdialog.h"
namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    QTcpSocket *tcpSocket;
    QString sentJSONArray;
signals:
    void sendJSON(QString a);
    void sendip(QString);
    void sendport(int);
    void sendnursename(QString);

public slots:
    void setip(QString);
    void setport(int);

private slots:
    void on_Bn_login_clicked();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void on_Bn_cancel_clicked();

    void sendJSONSlot();
    void on_Bn_network_clicked();

private:
    Ui::LoginDialog *ui;
    SethostDialog *sethostDlg;
    QPoint windowpos;
    QPoint mousepos;
    QPoint dpos;
    QString hostip;
    int hostport;

};

#endif // LOGINDIALOG_H
