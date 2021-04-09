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

  w.move(200, 100);
  w.show();

  return a.exec();
}
