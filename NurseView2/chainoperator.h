#ifndef CHAINOPERATOR_H
#define CHAINOPERATOR_H
#include "chainitem.h"

//static patientchain head;

class chainoperator
{
public:
    int insertitem(chainitem *currentpos);
    int deleteitem(chainitem *currentpos);

    int goThrough();
private:
    static chainitem *head;
};

#endif // CHAINOPERATOR_H
