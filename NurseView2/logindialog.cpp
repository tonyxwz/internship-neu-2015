#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMouseEvent>
#include <Qpoint>

LoginDialog::LoginDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::LoginDialog)
{
  ui->setupUi(this);
  // connect(this,SIGNAL(hide()),this,SLOT(sendJSON(QString)));
  setWindowFlags(Qt::FramelessWindowHint);
  this->hostip = "10.25.35.67";
  this->hostport = 2345;
  tcpSocket = new QTcpSocket(this);
  sethostDlg = new SethostDialog;
  connect(sethostDlg, SIGNAL(sendip(QString)), this, SLOT(setip(QString)));
  connect(sethostDlg, SIGNAL(sendport(int)), this, SLOT(setport(int)));
}

LoginDialog::~LoginDialog()
{
  delete ui;
}
void
LoginDialog::mousePressEvent(QMouseEvent* event)
{
  this->windowpos = this->pos();
  this->mousepos = event->globalPos();
  this->dpos = mousepos - windowpos;
}

void
LoginDialog::mouseMoveEvent(QMouseEvent* event)
{
  this->move(event->globalPos() - this->dpos);
}

void
LoginDialog::on_Bn_login_clicked()
{
  ui->Bn_login->setEnabled(false);
  if (this->hostip == "") {
    ui->Bn_login->setEnabled(true);
    QMessageBox::warning(this, tr("警告"), tr("请先设置服务器！"));
  } else {

    if ((ui->lE_username->text() == "") || (ui->LE_passwd->text() == "")) {
      QMessageBox::warning(this, tr("警告"), tr("请补全用户名或密码！"));
      ui->Bn_login->setEnabled(true);

    } else {
      tcpSocket->abort();
      // tcpSocket->connectToHost(tr("10.25.35.67"),2345);
      tcpSocket->connectToHost(hostip, hostport);
      if (tcpSocket->waitForConnected(5000)) {
        char* loginwelcome = (char*)malloc(sizeof(char) * 256);
        char username[10] = { 0 };
        char password[10] = { 0 };

        strcat(username, ui->lE_username->text().toStdString().c_str());
        strcat(password, ui->LE_passwd->text().toStdString().c_str());

        memset(loginwelcome, 0, sizeof(char) * 256);
        sprintf(loginwelcome,
                "{\"action\":\"1\",\"username\":\"%s\",\"password\":\"%s\"}",
                username,
                password);

        tcpSocket->write(loginwelcome, strlen(loginwelcome));

        memset(loginwelcome, 0, sizeof(char) * 256);
        if (tcpSocket->waitForReadyRead()) {
          // tcpSocket->waitForReadyRead();
          /************logic problem************/
          tcpSocket->getChar(loginwelcome);

          tcpSocket->close();
          // char* test_json =
          // "{\"num\":2,\"1\":{\"name\":\"sushao\",\"age\":\"20\",\"time\":\"2013
          // 2 2\"},\"2\":{\"name\":\"dage\",\"age\":\"30\",\"time\":\"2013 2
          // 2\"}}";
          if (!strcmp(loginwelcome, "1")) {
            // NurseView *nurseview = new NurseView;
            tcpSocket->connectToHost(hostip, hostport);
            if (tcpSocket->waitForConnected(5000)) {
              char* loggedin = (char*)malloc(sizeof(char) * 256);
              memset(loggedin, 0, sizeof(char) * 256);
              char* JSONArray = (char*)malloc(sizeof(char) * 4096);
              memset(JSONArray, 0, sizeof(char) * 4096);

              sprintf(
                loggedin, "{\"action\":\"2\",\"username\":\"%s\"}", username);
              // qDebug(loggedin);

              // qDebug() << sizeof(*loggedin);
              tcpSocket->write(loggedin, strlen(loggedin));
              tcpSocket->waitForReadyRead();
              tcpSocket->read(JSONArray, 4096);
              // qDebug() << JSONArray;
              sentJSONArray = JSONArray;
              //#############################

              emit sendJSON(sentJSONArray);
              emit sendip(hostip);
              emit sendport(hostport);
              emit sendnursename(ui->lE_username->text());

              tcpSocket->close();
              free(loggedin);
              free(JSONArray);
            }
            // nurseview->show();
            this->close();

          } else {
            ui->Bn_login->setEnabled(true);
            QMessageBox::warning(this, tr("错误!"), tr("用户名或密码错误！"));
          }

          // qDebug(loginwelcome);
          free(loginwelcome);
        }
      } else {
        QMessageBox::warning(
          this, tr("错误!"), tr("远程服务器未响应，请检查您的网络连接！"));
        ui->Bn_login->setEnabled(true);
      }
    }
  }
}

void
LoginDialog::on_Bn_cancel_clicked()
{
  sentJSONArray = "zhelikeyifasongxinhao";
  exit(1);
  /*
  emit sendJSON(sentJSONArray);
  this->close();
  */
}

void
LoginDialog::sendJSONSlot()
{
  QString a = "Signal 2 Caught!";
  emit sendJSON(a);
}

void
LoginDialog::on_Bn_network_clicked()
{
  sethostDlg->exec();
}

void
LoginDialog::setip(QString newip)
{
  this->hostip = newip;
  // qDebug() << hostip;
}

void
LoginDialog::setport(int newport)
{
  this->hostport = newport;
  // qDebug() << hostport;
}
