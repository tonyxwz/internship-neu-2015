#include "widget.h"
#include <QtGui/QApplication>

int
main(int argc, char* argv[])
{
  QApplication::setStyle("cleanlooks");
  QApplication a(argc, argv);
  Widget w;
  w.setWindowTitle("ClientLogin");
  w.setWindowOpacity(1);
  w.setWindowFlags(Qt::FramelessWindowHint);
  w.setAttribute(Qt::WA_TranslucentBackground);
  w.show();
  w.move(0, 0);
  return a.exec();
}
