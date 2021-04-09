#include "nurseviewwindow.h"
#include <QTextCodec>
#include <QtGui/QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QTextCodec* codec = QTextCodec::codecForName("GB2312");
  QTextCodec::setCodecForTr(codec);
  QTextCodec::setCodecForLocale(codec);
  QTextCodec::setCodecForCStrings(codec);
  NurseViewWindow w;
  w.show();

  return a.exec();
}
