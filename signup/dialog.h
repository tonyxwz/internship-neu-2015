#ifndef DIALOG_H
#define DIALOG_H
#include "chat-client.h"
#include <QDialog>
#include <QMouseEvent>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget* parent = 0);

  ~Dialog();

private slots:
  void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_lineEdit_3_cursorPositionChanged(int arg1, int arg2);
  void mousePressEvent(QMouseEvent*);
  // void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent*);

private:
  Ui::Dialog* ui;
  bool mousePressed;
  QPoint mousePos;
  QPoint windowPos;
  QPoint dPos;
  ChatClient client;
  QTcpSocket* socket;
};

#endif // DIALOG_H
