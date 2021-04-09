#include "chainitem.h"
#include "chainoperator.h"


int chainoperator::insertitem(chainitem *currentpos)
{
    if(1 == 0)
    {

    }
    return 0;
}

int chainoperator::deleteitem(chainitem *currentpos)
{
    if(1 == 0)
    {
        this->head = currentpos->getnext();
    }
    currentpos->deletenext();
    return 0;


}

int chainoperator::goThrough()
{
    //printf("mychain有%d %d项\n",sizeof(*mychain),sizeof(chainitem));
    return 0;

}




