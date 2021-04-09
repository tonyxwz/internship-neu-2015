#ifndef WIDGET_H
#define WIDGET_H

#include "dialog.h"
#include <QMouseEvent>
#include <QWidget>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget* parent = 0);
  ~Widget();

private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
  void mousePressEvent(QMouseEvent*);
  // void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent*);

private:
  Ui::Widget* ui;

private:
  bool mousePressed;
  QPoint mousePos;
  QPoint windowPos;
  QPoint dPos;
  bool b;
  Dialog* dialog;
};

#endif // WIDGET_H
