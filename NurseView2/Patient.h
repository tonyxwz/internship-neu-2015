#ifndef CHAINITEM_H
#define CHAINITEM_H
#include <Qstring>

//#include <QDebug>


//a chainitem is detail of one pateint;
class Patient
{
friend  class QlistWidget;
public:
    Patient();
    //~chainclass();
    int deletenext();
    int inserttohere(Patient *newitem);
    //int gothrough();
    int setpid(QString newpid);
    QString getpid();

    int setname(QString newname);
    QString getname();

    int setgender(QString newgender);
    QString getgender();

    int setage(QString newage);
    QString getage();

    int settime(QString newtime);
    QString gettime();

    int setdoctor(QString newdoctor);
    QString getdoctor();

    Patient* getnext();

    //int printpatient();

    //int set

private:
    QString pid;
    QString name;
    QString gender;
    QString age;
    QString time;
    QString doctor;
    Patient *next;


};

#endif // CHAINCLASS_H
