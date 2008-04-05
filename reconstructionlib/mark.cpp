#include "mark.h"


int Mark::idCounter=0;

Mark::
Mark() :  position(3)
{
    id=idCounter++;
    state=inaktive;
    idOfTermination=-1;
    labelIsEditable=true;
    sprintf(label,"label id:%d",id);
}

Mark::
Mark(Vector<int> &pos, State st, int idOfT) :  position(3)
{
    id=idCounter++;
    
    state=st;
    idOfTermination=idOfT;
    position=pos;
    labelIsEditable=true;
    sprintf(label,"label id:%d",id);

}


int
Mark::
operator=(Mark &lo)
{
    id=lo.id;
    state=lo.state;
    position=lo.position;
    idOfTermination=lo.idOfTermination;
    labelIsEditable=lo.labelIsEditable;
    sprintf(label," %s",lo.label);
    return 0;
}




int
Mark::
GetMarkLabel(char presentLabel[LABELLENGTH])
{
    
    sprintf(presentLabel," %s",label);
    return 0;
}






int 
Mark::
SetMarkLabel(char newLabel[LABELLENGTH])
{
    
    sprintf(label," %s",newLabel);
    return 0;
}

