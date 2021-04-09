#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <QPoint>
Widget::Widget(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::Widget)
{
  ui->setupUi(this);
  b = true;
  dialog = new Dialog;
}

Widget::~Widget()
{
  delete ui;
}

void
Widget::mousePressEvent(QMouseEvent* event)
{
  this->windowPos = this->pos();
  this->mousePos = event->globalPos();
  this->dPos = mousePos - windowPos;
}
void
Widget::mouseMoveEvent(QMouseEvent* event)
{
  this->move(event->globalPos() - this->dPos);
}
/*void Widget::mousePressEvent(QMouseEvent *lpEvent)
{
        Widget::mousePressEvent(lpEvent);
        if (lpEvent->button() == Qt::LeftButton)
        {
                m_WindowPos = this->pos();
                m_MousePos = lpEvent->globalPos();
                this->m_MousePressed = true;
        }
}

void Widget::mouseReleaseEvent(QMouseEvent *lpEvent)
{
        Widget::mouseReleaseEvent(lpEvent);
        if (lpEvent->button() == Qt::LeftButton)
        {
                this->m_MousePressed = false;
        }
}

void Widget::mouseMoveEvent(QMouseEvent *lpEvent)
{
        if (m_MousePressed)
        {
                this->move(m_WindowPos + (lpEvent->globalPos() - m_MousePos));
        }
}*/
void
Widget::on_pushButton_clicked()
{
  // Dialog *dialog=new Dialog;
  this->hide();
  // dialog->show();
  // dialog->hide();
  dialog->resize(800, 600);
  dialog->setModal(true); // mo tai
  dialog->exec();         // mo tai
}
void
Widget::on_pushButton_2_clicked()
{
  this->close();
}
