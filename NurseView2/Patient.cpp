#include "Patient.h"

Patient::Patient()
{

}

int Patient::deletenext()
{
    this->next = this->next->next;
    return 0;
}

int Patient::inserttohere(Patient *newitem)
{
    newitem->next = this->next;
    this->next = newitem;
    return 0;
}

int Patient::setpid(QString newpid)
{
    this->pid = newpid;
    return 0;
}

int Patient::setname(QString newname)
{
    this->name = newname;
    return 0;
}

int Patient::setgender(QString newgender)
{
    this->gender = newgender;
    return 0;

}

int Patient::setage(QString newage)
{
    this->age = newage;
    return 0;

}

int Patient::settime(QString newtime)
{
    this->time = newtime;
    return 0;

}

int Patient::setdoctor(QString newdoctor)
{
    this->doctor = newdoctor;
    return 0;

}

QString Patient::getpid()
{
    return this->pid;
}

QString Patient::getname()
{
    return this->name;
}

QString Patient::getgender()
{
    return this->gender;
}

QString Patient::getage()
{
    return this->age;
}

QString Patient::gettime()
{
    return this->time;
}

QString Patient::getdoctor()
{
    return this->doctor;
}

/*
int Patient::printpatient()
{
    qDebug() << "id" << this->getpid();
    qDebug() << "name:" << this->getname();
    qDebug() << "gender:" << this->getgender();
    qDebug() << "age:" << this->getage();
    qDebug() << "time:" << this->gettime();
    qDebug() << "doctor:" << this->getdoctor();
    return 0;
}
*/
