#include "sethostdialog.h"
#include "ui_sethostdialog.h"

SethostDialog::SethostDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::SethostDialog)
{
  ui->setupUi(this);
  ui->lineEdit_port->setVisible(false);
  ui->label_port->setVisible(false);
}

SethostDialog::~SethostDialog()
{
  delete ui;
}

void
SethostDialog::on_Bn_Connect_clicked()
{
  QString pattern("(((\\d{1,2})|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))\\.){3}((\\d{1,"
                  "2})|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))");
  QRegExp rx(pattern);
  bool matchip = rx.exactMatch(ui->lineEdit_ip->text());
  // bool matchport = (ui->lineEdit_port->text().toInt() <= 9999);
  // bool matchport2 = (ui->lineEdit_port->text().toInt() >= 1000);
  if (matchip) {
    emit sendip(ui->lineEdit_ip->text());
    // emit sendport(ui->lineEdit_port->text().toInt());
    this->close();
  } else {
    QMessageBox::warning(this, tr("警告"), tr("请输入合法的IP地址!"));
  }
}
