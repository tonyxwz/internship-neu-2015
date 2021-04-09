#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMessageBox>

#include <QtCore/QCoreApplication>
Dialog::Dialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::Dialog)

{
  ui->setupUi(this);
  ui->lineEdit_3->setValidator(new QIntValidator(0, 200, this));
  // ui->lineEdit_4->setValidator(new QIntValidator(0,99999999999, this));
  // ui->lineEdit_5->setValidator(new QIntValidator(0,99999999, this));
  QRegExp double_rx109("100|([0-9]{0,11}[.][0-9]{1,3})");
  ui->lineEdit_4->setValidator(new QRegExpValidator(double_rx109, this));
  QRegExp double_rx100("100|([0-9]{0,12}[.][0-9]{1,3})");
  ui->lineEdit_5->setValidator(new QRegExpValidator(double_rx100, this));

  this->setWindowOpacity(1);
  this->setWindowFlags(Qt::FramelessWindowHint);
  this->setAttribute(Qt::WA_TranslucentBackground);
  socket = new QTcpSocket();
  // ui->listWidget->setStyleSheet("border:1px;background-color:transparent");
}

Dialog::~Dialog()
{
  delete ui;
}

void
Dialog::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{}

void
Dialog::on_pushButton_clicked()
{
  if (ui->listWidget->currentRow() == -1) {
    QMessageBox::information(this, "alarm", "select department");
  } else {

    QString le1 = ui->lineEdit->text();
    QString le2 = ui->lineEdit_2->text();
    QString le3 = ui->lineEdit_3->text();
    QString le4 = ui->lineEdit_4->text();
    QString le5 = ui->lineEdit_5->text();
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    int ls5 = ui->listWidget->currentRow();
    char sle1[50] = { 0 }, sle2[50] = { 0 }, sle3[50] = { 0 }, sle4[50] = { 0 },
         sle5[50] = { 0 };
    strcat(sle1, le1.toStdString().c_str());
    strcat(sle2, le2.toStdString().c_str());
    strcat(sle3, le3.toStdString().c_str());
    strcat(sle4, le4.toStdString().c_str());
    strcat(sle5, le5.toStdString().c_str());
    char data[100] = { 0 };
    sprintf(data,
            "{\"le1\":\"%s\",\"le2\":\"%s\",\"le3\":\"%s\",\"le4\":\"%s\","
            "\"le5\":\"%s\",\"ls5\":\"%d\"}",
            sle1,
            sle2,
            sle3,
            sle4,
            sle5,
            ls5);
    qDebug(data);
    socket->connectToHost("10.25.35.67", 1234);
    socket->write(data, sizeof(data));
    socket->waitForReadyRead(-1);
    QByteArray newthing = socket->readAll();
    QMessageBox::information(this, "wrong", newthing);
    qDebug() << newthing;
    socket->close();
    // client.ConnectToServer( "10.25.35.67", 1234 );
    // client.waitForConnected( -1 );

    // char msg[1024]= { 0 };
    // while( 1 ) {
    //                gets( msg );
    //               if( 0 == strcmp(msg, "exit") )
    //                      break;
    //
    // if( !client.WriteData(data, strlen(data) + 1) ) {
    // printf( "WriteData failed !\n" );
    //  break;
    //}
    //         }

    // client.OnReadyRead();

    // client.Close();
  }
}

void
Dialog::on_pushButton_2_clicked()
{
  this->close();
}

void
Dialog::on_lineEdit_3_cursorPositionChanged(int arg1, int arg2)
{}
void
Dialog::mousePressEvent(QMouseEvent* event)
{
  this->windowPos = this->pos();
  this->mousePos = event->globalPos();
  this->dPos = mousePos - windowPos;
}
void
Dialog::mouseMoveEvent(QMouseEvent* event)
{
  this->move(event->globalPos() - this->dPos);
}
