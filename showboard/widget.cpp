#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <QMovie>
#include <QSize>

Widget::Widget(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::Widget)
{

  ui->setupUi(this);
  ui->nowline->setAlignment(Qt::AlignCenter);
  QTimer* timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(timeCountsFunction()));
  timer->start(1000);
  tcpSocket = new QTcpSocket(this);

  connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));

  connect(tcpSocket,
          SIGNAL(error(QAbstractSocket::SocketError)),

          this,
          SLOT(displayError(QAbstractSocket::SocketError)));

  QMovie* movie = new QMovie("./03.gif");
  movie->setScaledSize(QSize(100, 100));
  // ui->label->setGeometry(0,0,0,0);
  ui->label->setMovie(movie);
  movie->start();

  newConnect();
}

Widget::~Widget()
{
  delete ui;
}

void
Widget::timeCountsFunction()
{
  QDateTime time = QDateTime::currentDateTime(); //获取系统现在的时间

  QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式

  ui->timenow->setText(str);
}
void
Widget::newConnect()

{

  blockSize = 0; //初始化其为0

  tcpSocket->abort(); //取消已有的连接

  tcpSocket->connectToHost("10.25.35.67", 4567);

  //连接到主机，这里从界面获取主机地址和端口号
}
void
Widget::readMessage()

{
  // tcpSocket->waitForReadyRead(-1);
  char messag[100];
  int num = tcpSocket->read(messag, 100);

  messag[num] = '\0';

  //如果没有得到全部的数据，则返回，继续接收数据
  qDebug() << messag;

  //将接收到的数据存放到变量中
  ui->nowline->setText(messag);

  //显示接收到的数据
}
void Widget::displayError(QAbstractSocket::SocketError)

{

  qDebug() << tcpSocket->errorString(); //输出错误信息
}

/*void Widget::mousePressEvent(QMouseEvent *event){
        this->windowPos = this->pos();
        this->mousePos = event->globalPos();
       this->dPos = mousePos - windowPos;
}
void Widget::mouseMoveEvent(QMouseEvent *event){
        this->move(event->globalPos() - this->dPos);
}*/

void
Widget::on_pushButton_clicked()
{
  exit(0);
}

void
Widget::on_timenow_linkActivated(const QString& link)
{}
