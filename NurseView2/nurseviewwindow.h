#ifndef NURSEVIEWWINDOW_H
#define NURSEVIEWWINDOW_H

#include <QMainWindow>
#include <QtScript>
#include <QComboBox>
#include <QTcpSocket>

#include "logindialog.h"
#include "Patient.h"

namespace Ui {
    class NurseViewWindow;
}

class NurseViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NurseViewWindow(QWidget *parent = 0);
    void parsejson(QString a);
    ~NurseViewWindow();


public slots:
    void setJSON(QString);
    //void settcpsocket(QTcpSocket);
    void changed(QString position);

    void setip(QString);
    void setport(int);
    void setnursename(QString);

private slots:
    void on_pushbutton_clicked();

private:
    Ui::NurseViewWindow *ui;
    LoginDialog *lgd;
    Patient *patient;
    QTcpSocket *mytcp;
    QVector<QComboBox*> cbDianYa;
    QSignalMapper* signalMapper;

    QString hostip;
    int hostport;
    QString nursename;

    //QString JSONArray;

};

#endif // NURSEVIEWWINDOW_H
