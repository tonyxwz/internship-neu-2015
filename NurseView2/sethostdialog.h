#ifndef SETHOSTDIALOG_H
#define SETHOSTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QRegExp>

namespace Ui {
class SethostDialog;
}

class SethostDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SethostDialog(QWidget* parent = 0);
  ~SethostDialog();
signals:
  void sendip(QString);
  void sendport(int);

private slots:
  void on_Bn_Connect_clicked();

private:
  Ui::SethostDialog* ui;
};

#endif // SETHOSTDIALOG_H
