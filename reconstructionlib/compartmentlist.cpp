#include "compartmentlist.h"

// Function to support process bar
static int
EmptyCallBackFunction(int percentage)
{
    return 0;
};

// Static Variable to Identifie CompartmentList Unique
int CompartmentList::IDCounter=0;



//Constructor and Destruktor
CompartmentList::
CompartmentList(DataCube *dC,int Llevel) 
{
    kindOfNeigbourHood=6;
    body=NULL;
    bodyComp=NULL;
    dataCube=dC;
    level=Llevel;
    NrOfCompartments=0;
    id=IDCounter;
    IDCounter=IDCounter+1;
    callBackFunction=EmptyCallBackFunction;
};


CompartmentList::
CompartmentList(DataCube *dC,int Llevel,int (*callBack) (int percentage))
{
    kindOfNeigbourHood=6;
    body=NULL;
    bodyComp=NULL;
    dataCube=dC;
    level=Llevel;
    NrOfCompartments=0;
    id=IDCounter;
    IDCounter=IDCounter+1;
    callBackFunction=callBack;
};


CompartmentList::  
~CompartmentList()
{
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
	delete (*pos);

//////////////////////////     Shell Model             //////////////////////////////
    for (int i=0; i<maxDist; i++)
		for(pos=(routeList[i]).begin();pos!=(routeList[i]).end();++pos)
			delete (*pos); 
//////////////////////////////////////////////////////////////////////////////////////    
    
};


//Modify List of Compartments
int 
CompartmentList::
addElementFront(Compartment *comp)
{
    List.push_front(comp);
    NrOfCompartments++;
    return 0;
    
}

int 
CompartmentList::
addElementEnd(Compartment *comp)
{
    List.push_back(comp);
    NrOfCompartments++;		
    return 0;	
}

int
CompartmentList::
insertElement(list<Compartment*>::iterator pos,Compartment *comp)
{
    list<Compartment*>::iterator newpos;
    newpos=List.insert(pos,comp);
    NrOfCompartments++;
    return 0;
    
}

int 
CompartmentList::
deleteElement(list<Compartment*>::iterator pos)
{
    delete *pos;
    List.erase(pos);
    NrOfCompartments--;
    return 0;
}

list<Compartment*>::iterator 
CompartmentList::
eraseElement(list<Compartment*>::iterator pos)
{
    delete *pos;
    pos=List.erase(pos);
    NrOfCompartments--;
    return pos;
}


/*!
* Delets compartments if there is no structure on their surface or if they have no neigbours.
 */
int
CompartmentList::
reduceList(double epsilon)
{
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
        (*pos)->IsThereStructureOnMySurface(epsilon);
    for (pos=List.begin();pos!=List.end();++pos)
        (*pos)->AmIASingleOrDoIHaveNeigbours();
    
    
    for (pos=List.begin();pos!=List.end();++pos)
    {
        if ( ((*pos)->FLAG(STRUCTUREONMYSURFACE) ==0)  || ((*pos)->FLAG(SINGLE)) )
        {
            //Here is a memory leak, the erased element has to be deleted !!!
            pos=eraseElement(pos);
            if (pos!=List.begin()) pos--;
        };
    };
    
    return 0;
};


//Create Neigbourhood
int
CompartmentList::
EvaluateAndSetNeigboursForList()
{
    
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
        (*pos)->FindAndSetMyNeigbours6();
    kindOfNeigbourHood=6;
    return 0;
    
}



//////////////////////////  Shell Model   ///////////////////////////////////////////////
// function arranges the list beginning with the compartments with the smallest distance
// to the cell body to those with the biggest distance

int
CompartmentList::
arrangeList()
{
    list<Compartment*>::iterator pos;
  
    for (pos=List.begin();pos!=List.end();++pos)
    	if((*pos)->FLAG(NOTCONNECTEDTOBODY)!=NOTCONNECTEDTOBODY)
			if((*pos)->GetShellModelDistanceFromBody()>=maxDist)
				maxDist=(*pos)->GetShellModelDistanceFromBody();
	
  
    routeList=new(list<Compartment*>[maxDist+1]);


    for (pos=List.begin();pos!=List.end();++pos)
		if((*pos)->FLAG(NOTCONNECTEDTOBODY)!=NOTCONNECTEDTOBODY)	
        	(routeList[(*pos)->GetShellModelDistanceFromBody()]).push_back(*pos);
          
    return 0;
    
}
//////////////////////////////////////////////////////////////////////////////////////////





//Graph extraction


/*!
* This functions calcualtes the distance from the body compartment. Therefore it creates a marching front
 * which starts at the body compartment and spreads along the neigbourhood of the compartments
 * Uses a 6 neigbourhood to calcultae the distance!
 */
int
CompartmentList::
CalcualteAndSetDistancesFromBodyCompartment()
{
    Compartment *Origin=bodyComp;
    if (Origin==NULL) return 1;
    
    int dist;
    list<Compartment*>::iterator pos;
    list<Compartment*> frontList;
    frontList.push_front(Origin);
    Origin->SetDistanceFromBody(0);
    for(int j=0;((j<10000) && (!frontList.empty()));j++)
    {
        for (pos=frontList.begin();pos!=frontList.end();++pos)
        {
	    
            dist=(*pos)->GetDistanceFromBody();
            dist++;
            int kind;
            kind=N_Top;
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                {
                    ((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
                    frontList.push_front((*pos)->GetConnection(kind));
                }
                    kind=N_Right;
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                {
                    ((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
                    frontList.push_front((*pos)->GetConnection(kind));
                }
                    kind=N_Back;
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                {
                    ((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
                    frontList.push_front((*pos)->GetConnection(kind));
                }
                    kind=N_Left;
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                {
                    ((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
                    frontList.push_front((*pos)->GetConnection(kind));
                }
                    kind=N_Front;
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                {
                    ((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
                    frontList.push_front((*pos)->GetConnection(kind));
                }
                    kind=N_Bottom;
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                {
                    ((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
                    frontList.push_front((*pos)->GetConnection(kind));
                }
		    
		    
                    pos=frontList.erase(pos);
            pos--;
	    
	    
        }
	
    };
    
    for (pos=List.begin();pos!=List.end();++pos)
        if  ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME) (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
    
    
    return 0;
}


/*!
* This functions calcualtes the distance from the body compartment. Therefore it creates a marching front
 * which starts at the body compartment and spreads along the neigbourhood of the compartments
 * Uses a 27 neigbourhood to calcultae the distance!
 */
int
CompartmentList::
CalcualteAndSetDistancesFromBodyCompartment27()
{
    Compartment *Origin=bodyComp;
    if (Origin==NULL) return 1;
    
    int dist;
    list<Compartment*>::iterator pos;
    list<Compartment*> frontList;
    frontList.push_front(Origin);
    Origin->SetDistanceFromBody(0);
    for(int j=0;((j<10000) && (!frontList.empty()));j++)
    {
        for (pos=frontList.begin();pos!=frontList.end();++pos)
        {
	    
            dist=(*pos)->GetDistanceFromBody();
            dist++;
            int kind;
            kind=N_Top;
            for (kind=N_TopFront;kind<=N_Bottom;kind++)
            {
		if ((*pos)->GetConnection(kind)!=NULL)
		    if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
		    {
			((*pos)->GetConnection(kind))->SetDistanceFromBody(dist);
			frontList.push_front((*pos)->GetConnection(kind));
		    }
            };
		
		
		
		pos=frontList.erase(pos);
		if (pos!=frontList.begin()) pos--;
		
        };
	    
    };
	
	for (pos=List.begin();pos!=List.end();++pos)
	    if  ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME) (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
	
	
	return 0;
}


/*!
* This functions calcualtes the distance from the body compartment. Therefore it creates a marching front
 * which starts at the body compartment and spreads along the neigbourhood of the compartments
 * Uses a 27 neigbourhood to calcultae the distance!
 * The distances are 10 for the six neigbourhood about sqrt(1) * 10
 *		     14 for the eigtheen neigbour hood about sqrt(2) *10 
 * 		     17 for the tweenty seven neigbour hood about sqrt(3) *10
 *  see Borgefors G., Distances Transforms in arbitrary dimension., Computer Vision, Graphics and Image Processing 27 (19984), 321 - 345
 */

//help function
int
CompartmentList::
ChamferDistance101417(int kind)
{
    switch (kind)
    {
        //top Front/Back Right/Left
        case N_TopFront      	: return 14; break;//1
        case N_TopBack       	: return 14; break;//2
        case N_TopFrontRight 	: return 17; break;//1
        case N_TopRight      	: return 14; break;//3
        case N_TopLeft       	: return 14; break;//4
        case N_TopFrontLeft  	: return 17; break;//2
        case N_TopBackLeft   	: return 17; break;//3
        case N_TopBackRight  	: return 17; break;//4
        case N_Top           	: return 10; break;//1
						 //samelevel
        case N_Right         	: return 10; break;//2
        case N_FrontRight    	: return 14; break;//5
        case N_Back          	: return 10; break;//3
        case N_Left          	: return 10; break;//4
        case N_FrontLeft     	: return 14; break;//6
        case N_BackLeft      	: return 14; break;//7
        case N_BackRight        : return 14; break;//8
        case N_Front            : return 10; break;//5
						   //bottom
        case N_BottomRight      : return 14; break;//9
        case N_BottomFrontRight : return 17; break;//5
        case N_BottomBack  	: return 14; break;//10
        case N_BottomLeft  	: return 14; break;//11
        case N_BottomFrontLeft  : return 17; break;//6
        case N_BottomBackLeft  	: return 17; break;//7
        case N_BottomBackRight  : return 17; break;//8
        case N_BottomFront     	: return 14; break;//12
        case N_Bottom    	: return 10; break;//6 
    }
    // 6 times six neigbourhood
    //12 times eigtheen neigbourhood
    // 8 times twonty six (seven) neigbourhood 
    
    cout << "Error, nothing known about kind:" << kind << endl;
    return -1;
}


int
CompartmentList::
CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction()
{
    Compartment *Origin=bodyComp;
    if (Origin==NULL) return 1;
    
    int dist;
    list<Compartment*>::iterator pos;
    list<Compartment*> frontList;
    frontList.push_front(Origin);
    Origin->SetDistanceFromBody(0);
    for(int j=0;((j<10000) && (!frontList.empty()));j++)
    {
        for (pos=frontList.begin();pos!=frontList.end();++pos)
        {
	    
            dist=(*pos)->GetDistanceFromBody();
            int kind;
            kind=N_Top;
            for (kind=N_TopFront;kind<=N_Bottom;kind++)
            {
                if ((*pos)->GetConnection(kind)!=NULL)
                {
                    int nextdist=dist+ChamferDistance101417(kind);
                    if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>nextdist)
                    {
                        ((*pos)->GetConnection(kind))->SetDistanceFromBody(nextdist);
                        frontList.push_front((*pos)->GetConnection(kind));
                    }
                };
            };
	    
	    
	    
	    pos=frontList.erase(pos);
	    if (pos!=frontList.begin()) pos--;
	    
        };
	
    };
    
    for (pos=List.begin();pos!=List.end();++pos)
	if  ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME) (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
    
    
    return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////
/////                         shell model
////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// every neighbour belonging to a cube around a point get the distance 1 
int
CompartmentList::
shelModelDistance(int kind)
{
    if (kind >= N_TopFront && kind <= N_Bottom)
	return 1;
    else 
	return 0;
}

/*!
* This functions calculates the distance from the body compartment. Therefore it creates a marching front
* which starts at the body compartment and spreads along the neigbourhood of the compartments
* Uses a 27 neigbourhood to calculate the distance!
*/

int
CompartmentList::
CalcualteAndSetDistancesFromBodyCompartment_shellModel()
{
    Compartment *Origin=bodyComp;
    if (Origin==NULL) return 1;

    int dist;
	list<Compartment*>::iterator pos;
    list<Compartment*> frontList;
    frontList.push_front(Origin);
    Origin->SetShellModelDistanceFromBody(0);
    maxDist=0;
	
	for(int j=0;((j<10000) && (!frontList.empty()));j++)
    {
        for (pos=frontList.begin();pos!=frontList.end();++pos)
        {

            dist=(*pos)->GetShellModelDistanceFromBody();
	    for (int kind=N_TopFront;kind<=N_Bottom;kind++)
            {
                if ((*pos)->GetConnection(kind)!=NULL)
                {
                    int nextdist=dist+1;
					if(nextdist>maxDist)
						maxDist=nextdist;
                    if (((*pos)->GetConnection(kind))->GetShellModelDistanceFromBody()>nextdist)
                    {
                        ((*pos)->GetConnection(kind))->SetShellModelDistanceFromBody(nextdist);
                        frontList.push_front((*pos)->GetConnection(kind));
                    }
		};
            };
                pos=frontList.erase(pos);
                if (pos!=frontList.begin()) pos--;

	};

    };
 
        for (pos=List.begin();pos!=List.end();++pos)
            if  ((*pos)->GetShellModelDistanceFromBody()>=MAXARRIVALTIME) (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
			
			
}
	
////////////////          Calculation of the shell index       ///////////////

int
CompartmentList::
CalcualteAndSetIndex_shellModel()
{
    list<Compartment*>::iterator pos;
    list<Compartment*> frontList;
    int index=0;
    arrangeList();
       
    (*(routeList[0]).begin())->AddShellIndex(index);
    
    for(int i=1; i<=maxDist; i++)
    { 
	// initialise with the index -1
	for (pos=(routeList[i]).begin();pos!=(routeList[i]).end();++pos)
	    (*pos)->AddShellIndex(-1);
 

 	for (pos=(routeList[i]).begin();pos!=(routeList[i]).end();++pos)
 	{	 
	    list<int> helplist;
	    helplist=(*pos)->GetShellIndex();
 	    if (*helplist.begin()==-1)
 		{
		    frontList.push_front(*pos);
 		    index=index+1;
		     
 		    for (int kind=N_TopFront;kind<=N_Bottom;kind++)
  		    {
  			if ((*pos)->GetConnection(kind)!=NULL && ((*pos)->GetConnection(kind))->GetShellModelDistanceFromBody()==i-1)
  			    (*pos)->SetShellIndex((*pos)->GetConnection(kind)->GetShellIndex());
  		    } 
		
		    (*pos)->AddShellIndex(index);


		    // function sets shell_index of all neighbours equal to the one of frontList.begin()
		    frontList=SetNeighbourShellIndex(frontList, i);
		   
		}
	}
		 
    }


//////////////////////   Ausgabe   /////////////////////////////
/*  list<int>::iterator outpos;
    list<Compartment*>::iterator position;
    int k, i;
    int counter;
    
    for(int i=1; i<=maxDist; i++)
    {
	counter=0;
	cout<<"   i:  "<<i<<"     ";
	if(routeList[i].empty())
	    cout<<"Liste leer!"<<endl;
	else
	    for (position=(routeList[i]).begin();position!=(routeList[i]).end();position++)
	    {
		counter++;
		if(((*position)->GetShellIndex()).empty())
		    cout<<"Shell Liste leer!"<<endl;  
 		    else
		    {
			list<int> helplist;
			helplist = (*position)->GetShellIndex();
					
			for (outpos=helplist.begin(); outpos!=helplist.end(); outpos++) 
			{
			    cout<<(*outpos)<<"-";
			}
			cout<<endl;
		    }
		cout<<endl;
	    }
	cout<<"Anzahl der Elemente mit Abstand: "<<i<<"  "<<counter<<endl;
    } 
    
 	cout<<endl<<endl<<endl;
*/
    
    
    
    return 0;
}

list<Compartment*>
CompartmentList::
SetNeighbourShellIndex(list<Compartment*> frontList, int i)
{
    list<Compartment*>::iterator pos;

    while(!frontList.empty())
	for (pos=frontList.begin();pos!=frontList.end();++pos)
	{
	    for (int kind=N_TopFront;kind<=N_Bottom;kind++)
	    {
		if ((*pos)->GetConnection(kind)!=NULL 
		    && *((((*pos)->GetConnection(kind))->GetShellIndex()).begin())==-1
		    && ((*pos)->GetConnection(kind))->GetShellModelDistanceFromBody()==i) 
		{
		    ((*pos)->GetConnection(kind))->SetShellIndex((*pos)->GetShellIndex());
		    frontList.push_front((*pos)->GetConnection(kind));
		}
	    }
	    pos=frontList.erase(pos);
	    if (pos!=frontList.begin()) pos--;
	}
		    
    return frontList;
}


// detects cellbody with the help of the shel model and
// a permitted percentage of compartments which are no more directly contected in an shell 
int
CompartmentList::
DetectCellbody(list<Vector<double> > *cellbodyList, double percentage)
{
	list<Compartment*>::iterator pos;
	int ind, maxnumber;						// Index
	int maxBodyRadius=0;
	int connected=0;
	int abscounter=0;						// number of all compartments of a specified distance
	int cellIndex;
	
	int end=0;
	int* help;
	
	// first component of the int array gives the index, second one the number of compartments with this index
	// to a specified distance
	list<int*> counterList;    
    list<int*>::iterator countPos;
	
	
	int i=0; 
	while(end==0 && i<maxDist)
	{
			i++;
			maxnumber=0;
			abscounter=(routeList[i]).size();
		
			if(routeList[i].empty())
				cout<<"Liste leer!1"<<endl;
			else
			{
			
				for (pos=(routeList[i]).begin();pos!=(routeList[i]).end();++pos)
				{
					connected=0;
					ind=*((((*pos)->GetShellIndex()).begin()));
								
					// is index already in list?
					for(countPos=counterList.begin(); countPos!=counterList.end(); countPos++)
					{
						if((*countPos)[0]==ind)
						{
							(*countPos)[1]++;
							connected=1;
						}
					}
				
					//index is not yet in list		
					if(connected==0)
						{
						help=new int[2];
						help[0]=ind;
						help[1]=1;
						counterList.push_front(help);
						}
				}

				cout<<"Distanz:   "<<i<<endl;
				for (countPos=counterList.begin(); countPos!=counterList.end(); countPos++
				)
					if(counterList.empty())
						cout<<"Liste leer!2"<<endl;
					else
					{
						while(!counterList.empty())
							for (countPos=counterList.begin(); countPos!=counterList.end(); ++countPos)
							{
							//cout<<"Anzahl mit selbem Index:   "<<(*countPos)[1]<<"Index:  "<<(*countPos)[0]<<endl;
							if(maxnumber<=(*countPos)[1])
								{
								maxnumber=(*countPos)[1];
								cellIndex=(*countPos)[0];
								}
										
							countPos=counterList.erase(countPos);
							if (countPos!=counterList.begin()) countPos--;
							}
							cout<<endl<<endl;
					}
				
				//cout<<"Index:  "<<i<<"  Maxnumber  "<<maxnumber<<"  Abs:  "<<abscounter<<endl;	
				
				if(	!counterList.empty())
				cout<<"Index:   "<<i<<"Oops, das ist was schief gelaufe!"<<endl;
				
				
				if(((double)maxnumber/(double)abscounter)*100.>percentage)
				{	for (pos=(routeList[i]).begin();pos!=(routeList[i]).end();++pos)
						if(*((((*pos)->GetShellIndex()).begin()))==cellIndex)   
							{
								(*pos)->SETFLAG(BODYELEMENT);
								if(maxBodyRadius<=(*pos)->GetShellModelDistanceFromBody())
									maxBodyRadius=(*pos)->GetShellModelDistanceFromBody();
							}
				}			
				else
				{	end=1;}
			}
		}


Vector<int> start(3); 
Vector<double> spacing(3);
spacing = dataCube->GetSpaceing();
Vector<double> *position;
cout<<"sind wir noch da?"<<endl;
for(int i=1; i<=maxBodyRadius; i++)
    {
		if(routeList[i].empty())
			cout<<"Liste leer! bei Index"<<i<<endl;
		else
		{
			for (pos=(routeList[i]).begin();pos!=(routeList[i]).end();++pos)
			{
			 	if( (*pos)->FLAG(BODYELEMENT)==BODYELEMENT)
		 		{
			  		position=new Vector<double>(4);
			 		start=(*pos)->GetStart();
			 		(*position)[1]=(double)start[1]*spacing[1];
			 		(*position)[2]=(double)start[2]*spacing[2];
			 		(*position)[3]=(double)start[3]*spacing[3];
			 		(*position)[4]=(*pos)->CalculateRadius(1.0, 1);
			 		(*cellbodyList).push_front((*position));
			 	}
			 						
			}
		 }
	  }
	 
	return 0;
}




int
CompartmentList::
SearchLokalMaximaAndCreateButtonCompList_ShellModel()
{
    int dist;
    cout << "Start detection of dendrite terminations with the shell model" << endl;
    list<Compartment*>::iterator pos;
    int counter=0;
    for (pos=List.begin();pos!=List.end();pos++)
    {
        counter++;
        callBackFunction((100*counter)/NrOfCompartments);
        
        if ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME)
        {
	    (*pos)->CLEARFLAG(ENDPOINT);
            (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
            continue;
        }


        (*pos)->SETFLAG(ENDPOINT);
        dist=(*pos)->GetShellModelDistanceFromBody();

	for (int kind=N_TopFront;kind<=N_Bottom;kind++)
	    if ((*pos)->GetConnection(kind)!=NULL)
		if (((*pos)->GetConnection(kind))->GetShellModelDistanceFromBody()>dist)
		    (*pos)->CLEARFLAG(ENDPOINT);

     	if ((*pos)->FLAG(ENDPOINT))
            buttonCompList.push_back((*pos));

    }

    cout << "Detected " << buttonCompList.size() << " LokalMaxima." << endl;
    return 0;
}


int
CompartmentList::
CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther_ShellModel(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm)
{
	list<Compartment*>::iterator pos;
	list<Compartment*>::iterator pos2;
   	
	typedef list<Compartment*> Liste;
	list<Liste> connectionPlan;
    	list<Liste>::iterator Iter;
		
	int dist, dist2;
	int firstIndex, firstIndex2;
    	list<int> helplist;

    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
    {
////////////////   minimal distance from cellbody    //////////////////////////	
//         //Calculate necessary information
//         geometricDistanceFromBody=0;
//         help=(bodyComp->GetStart())-((*pos)->GetStart());
//         geometricDistanceFromBody=dataCube->DCNorm(help[1],help[2],help[3]);

//         //interpret information
//         //criteria 1:
//         if (geometricDistanceFromBody<minDistanceFromBodyCompartment)
//         {
//             cout << "Compartment:" << endl << (**pos).GetID()  << " is not meaningfull because termination, to tight at cellbody." << endl;
//             (*pos)->CLEARFLAG(ENDPOINT);
//             pos=buttonCompList.erase(pos);
//             if (pos!=buttonCompList.begin()) pos--;
//             continue;
//         };
/////////////////////////////////////////////////////////////////////////////////
	
	dist=(*pos)->GetShellModelDistanceFromBody();
	firstIndex=*((*pos)->GetShellIndex()).begin();

	list<Compartment*> isConnected;
	
	for(pos2=buttonCompList.begin(); pos2!=buttonCompList.end();++pos2)
	{
	    dist2=(*pos2)->GetShellModelDistanceFromBody();
	    firstIndex2=*((*pos2)->GetShellIndex()).begin();
	
	    if(dist==dist2)
		if(firstIndex2==firstIndex)
		  	{
                            isConnected.push_front(*pos2);
                            pos2=buttonCompList.erase(pos2);
                            if (pos2!=buttonCompList.begin()) pos2--;
                        }
	}
	
	connectionPlan.push_front(isConnected);
    }


	cout<<endl<<endl<<"Ausgabe:"<<endl;
    	for(Iter=connectionPlan.begin(); Iter!=connectionPlan.end(); ++Iter)
    	{
        	for(pos2=(*Iter).begin(); pos2!=(*Iter).end(); ++pos2)
            	cout<<(*pos2)->GetID()<<endl;
                cout<<endl<<endl;
    	}



//      int help=0;
//      if(!isConnected.empty())
//          for(pos2=isConnected.begin(); pos2!=isConnected.end(); pos2++)
//              for (int kind=N_TopFront;kind<=N_Bottom;kind++)
//              {
                    //    Compartment* neighbour=(*pos)->GetConnection(kind);
                    //    Compartment* buttom=*pos2;
                    //    if (neighbour->GetID()==buttom->GetID())
                    //  help++;
//              }
//      cout<<"This is your help!  "<<help<<endl;
//      if(help==1)
//      {
//          cout << "Compartment:" << endl << (**pos).GetID()  << " is not meaningfull because it's shell_connected to another one." << endl;
//          (*pos)->CLEARFLAG(ENDPOINT);
//          pos=buttonCompList.erase(pos);
//          if (pos!=buttonCompList.begin()) pos--;
//          continue;
//      }
//
//    }


  

    for(pos2=buttonCompList.begin(); pos2!=buttonCompList.end();++pos2)
    {    
        ResetNrOfPassingThroughAndBifurcationFlag();
      	
	cout << "Finnaly ther are " << buttonCompList.size() << "Dendrite Terminations" << endl;
	
	cout << "Detected " << buttonCompList.size() << " dendrite terminations" << endl;
	return 0;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////



/*!
* Searches for local maxima of distance in the compartments, checks 6 neigbourhood
 * these maxima should coresspondend with the dendrite terminations
 */
int
CompartmentList::
SearchLokalMaximaOfDistance6AndCreateButtonCompList()
{
    int dist;
    
    cout << "Start detection of dendrite terminations using 6 neigbourhood." << endl;
    list<Compartment*>::iterator pos;
    int counter=0;
    for (pos=List.begin();pos!=List.end();pos++)
    {
        counter++;
        callBackFunction((100*counter)/NrOfCompartments);
        
        if ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME)
        {
            //cout << "This compartment is not connected, it will be ignored" << (*pos) << endl;
            (*pos)->CLEARFLAG(ENDPOINT);
            (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
            continue;
	    
        }
        (*pos)->SETFLAG(ENDPOINT);
        dist=(*pos)->GetDistanceFromBody();
        int kind;
        kind=N_Top;
        if ((*pos)->GetConnection(kind)!=NULL)
            if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                (*pos)->CLEARFLAG(ENDPOINT);
	
        kind=N_Right;
        if ((*pos)->GetConnection(kind)!=NULL)
            if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                (*pos)->CLEARFLAG(ENDPOINT);
	
        kind=N_Back;
        if ((*pos)->GetConnection(kind)!=NULL)
            if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                (*pos)->CLEARFLAG(ENDPOINT);
	
        kind=N_Left;
        if ((*pos)->GetConnection(kind)!=NULL)
            if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                (*pos)->CLEARFLAG(ENDPOINT);
	
        kind=N_Front;
        if ((*pos)->GetConnection(kind)!=NULL)
            if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                (*pos)->CLEARFLAG(ENDPOINT);
	
        kind=N_Bottom;
        if ((*pos)->GetConnection(kind)!=NULL)
            if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                (*pos)->CLEARFLAG(ENDPOINT);
	
        if ((*pos)->FLAG(ENDPOINT))
            buttonCompList.push_back((*pos));
	
	
    }
    
    cout << "Detected " << buttonCompList.size() << " LokalMaxima using 6 neigbourhood.." << endl;
    return 0;
}


int
CompartmentList::
CheckIfThereIsInTheRNeigbourHoodABiggerDistance(Compartment *comp,double radius)
{
    
    list<Compartment*>::iterator pos;
    int x=comp->GetStart()[1];
    int y=comp->GetStart()[2];
    int z=comp->GetStart()[3];
    int distance=comp->GetDistanceFromBody();
    for (pos=List.begin();pos!=List.end();++pos)
    {
	if (*pos==comp) continue;
	if ((*pos)->FLAG(NOTCONNECTEDTOBODY)) continue;
	if (distance<(*pos)->GetDistanceFromBody())
	    if (dataCube->DCNorm(x-((*pos)->GetStart()[1]), y-((*pos)->GetStart()[2]), z-((*pos)->GetStart()[3]))<radius )
		return 1;
    }
    return 0;
}









/*!
* Checks if elements in buttonCompList are meaningfull if not they are removed from the list
 * criteria are:
 * 1.distance from cellbody (usually dendrite terminations are at least 10qm from body comp
			     * 2.
			     *
			     */
int
CompartmentList::
CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther(double minDistanceFromBodyCompartment,double minLengthOfDendriteInqm)
{
    list<Compartment*>::iterator pos;
    double geometricDistanceFromBody;
    Vector<int> help(3);
    
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
    {
        //Calculate necessary information
        geometricDistanceFromBody=0;
        help=(bodyComp->GetStart())-((*pos)->GetStart());
        geometricDistanceFromBody=dataCube->DCNorm(help[1],help[2],help[3]);
	
        //interpret information
        //criteria 1:
        if (geometricDistanceFromBody<minDistanceFromBodyCompartment)
        {
            cout << "Compartment:" << endl << (**pos).GetID()  << " is not meaningfull because termination, to tight at cellbody." << endl;
            (*pos)->CLEARFLAG(ENDPOINT);
            pos=buttonCompList.erase(pos);
            if (pos!=buttonCompList.begin()) pos--;
            continue;
        };
	
        //criteria 2:
        if (CheckIfThereIsInTheRNeigbourHoodABiggerDistance(*pos,minLengthOfDendriteInqm))
        {
            cout << "Compartment:" << endl << (**pos).GetID() << endl << "is not meaningfull because a in " << minLengthOfDendriteInqm << "qm distance is bigger." << endl;
            (*pos)->CLEARFLAG(ENDPOINT);
            pos=buttonCompList.erase(pos);
            if (pos!=buttonCompList.begin()) pos--;
            continue;
        };
	
        
    };
    
    //Check if tight neigbours exist
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
    {
        Vector<int> midpoint1=(*pos)->GetStart()+1/10*(5*(*pos)->GetSize());
        list<Compartment*>::iterator pos2;
        double mindist=((*pos)->GetSize())[1]*2;
	for (pos2=buttonCompList.begin();pos2!=buttonCompList.end();++pos2)
	    if (pos!=pos2)
	    {
		Vector<int> midpoint2=(*pos2)->GetStart()+1/10*(5*(*pos2)->GetSize());
		if (Norm(midpoint1-midpoint2)<mindist)
		{
		    cout << "Compartment:" << endl << (**pos2).GetID() << endl << "is not meaningfull because to tight to another one." << endl;
		    (*pos2)->CLEARFLAG(ENDPOINT);
		    pos2=buttonCompList.erase(pos2);
		    if (pos2!=buttonCompList.begin()) pos2--;
		    
		    
		    
		}
	    }	
		
		
    }    
	
	
	ResetDistancesFromBody();
	CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction();
	cout << "Finally detected " << buttonCompList.size() << " dendrite terminations" << endl;
	return 0;
}




int
CompartmentList::
MarkBoundaryCompartments()
{
    cout << "Mark boundary compartments" << endl;
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
    {
        (*pos)->AmIABoundaryComp();
    }
    return 0;
};

int
CompartmentList::
MarkBoundaryCompartments(short neigbourhoodType)
{
    cout << "Mark boundary compartments with neigbourhoodType:" << neigbourhoodType << endl;
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
    {
        (*pos)->AmIABoundaryComp(neigbourhoodType);
    }
    return 0;
};




int
CompartmentList::
extractSkeleton()
{
    
    
    
    int infocounter=0;
    list<Compartment*>::iterator pos;
    int counter=1;
    int counter2=1;
    cout << "Start to extract... " << endl;
    
    if (kindOfNeigbourHood==6)
    {
	Skeletonization skel;
        cout << "Skeletonization starts from 6 Neigbourhood list" << endl;
        while(counter>0)
        {
            callBackFunction(infocounter);
            infocounter+=10;
            if (infocounter>100) infocounter=0;
            counter=0;
	    
            for (pos=List.begin();pos!=List.end();++pos)
                (*pos)->AmIABoundaryComp();//sets boundary compartments
		
                for (pos=List.begin();pos!=List.end();++pos)
                    if ( (*pos)->FLAG(BOUNDARYCOMP) && (!((*pos)->FLAG(BODY)))	&& (!((*pos)->FLAG(ENDPOINT))) && (!((*pos)->FLAG(NOTCONNECTEDTOBODY))))
                    {
			
                        if ((*pos)->FLAG(ENDPOINT)) {cout << "Error, try to delete end point!" << endl;exit(1);};
			
			
                        if (skel.IsCompartmentDeleteable((*pos)) )
                        {
                            pos=eraseElement(pos);
                            if (pos!=List.begin()) pos--;
                            counter++;
                        };
                    }
			cout << "Deleted " << counter << " comps." << endl;
		
		
                
		
        }
    }
    else
    {
        cout << "Check neigbourhood" << endl;
	Skeletonization skel(26);
        int i=0;
        cout << "Skeletonization starts from 26 Neigbourhood list" << endl;
        while(counter>0)
        {
            i++;
            callBackFunction(infocounter);
            infocounter+=10;
            if (infocounter>100) infocounter=0;
            counter=0;
	    
            for (pos=List.begin();pos!=List.end();++pos)
                (*pos)->AmIABoundaryComp();//sets boundary compartments
		
                for (pos=List.begin();pos!=List.end();++pos)
                    if ( (*pos)->FLAG(BOUNDARYCOMP) && (!((*pos)->FLAG(BODY)))	&& (!((*pos)->FLAG(ENDPOINT))) && (!((*pos)->FLAG(NOTCONNECTEDTOBODY))))
                    {
			
                        if ((*pos)->FLAG(ENDPOINT)) {cout << "Error, try to delete end point!" << endl;exit(1);};
			
			
                        if (skel.IsCompartmentDeleteable26((*pos)) )
                        {
                            pos=eraseElement(pos);
                            if (pos!=List.begin()) pos--;
                            counter++;
                        };
                    }
                        cout << "Deleted " << counter << " comps." << endl;
		
                cout << "Second round with same boundary" << endl;
                counter2=0;
                for (pos=List.begin();pos!=List.end();++pos)
                    if ( (*pos)->FLAG(BOUNDARYCOMP) && (!((*pos)->FLAG(BODY)))	&& (!((*pos)->FLAG(ENDPOINT))) && (!((*pos)->FLAG(NOTCONNECTEDTOBODY))))
                    {
			
                        if ((*pos)->FLAG(ENDPOINT)) {cout << "Error, try to delete end point!" << endl;exit(1);};
			
			
                        if (skel.IsCompartmentDeleteable26((*pos)) )
                        {
                            pos=eraseElement(pos);
                            if (pos!=List.begin()) pos--;
                            counter2++;
                        };
                    }
                        cout << "Deleted in second round  " << counter2 << " comps." << endl;
                
		
                
                
        }
    }
    
    
    callBackFunction(100);
    return 0;
};




/*!
* Extracts the skeleton and tries to avoid to build tunels.
 * But actually there is still a problem for instance try the Y example.
 */

int
CompartmentList::
extractSkeleton2()
{
    int infocounter=0;
    list<Compartment*>::iterator pos;
    int counter1=1;
    int counter2=1;
    int counter3=1;
    int i=0;
    cout << "Start to extract... " << endl;
    
    
    Skeletonization skel(26);
    
    cout << "Skeletonization starts from 26 Neigbourhood list" << endl;
    while(counter1>0)
    {
        counter2=1;
        while(counter2>0)
        {
            counter3=1;
            while(counter3>0)
            {
                counter3=0;
		
                MarkBoundaryCompartments(6);
                for (int nrOfN=1;nrOfN<=6;nrOfN++)
                    for (pos=List.begin();pos!=List.end();++pos)
                        if ( ( (*pos)->GetNrOfNeigbours()<=nrOfN) && ((*pos)->FLAG(BOUNDARYCOMP)) && (!((*pos)->FLAG(BODY))) && (!((*pos)->FLAG(ENDPOINT))) && (!((*pos)->FLAG(NOTCONNECTEDTOBODY))))
                            if (skel.IsCompartmentDeleteable26((*pos)) )
                            {
                                pos=eraseElement(pos);
                                if (pos!=List.begin()) pos--;
                                counter3++;
                            };
                cout << "Deleted in 6 Neigbourhood with less then 2 neigours:" << counter3 << " comps." << endl;
                i++;
		
            }
	    
	    
	    
            counter2=0;
            MarkBoundaryCompartments(18);
            for (pos=List.begin();pos!=List.end();++pos)
                if ( (*pos)->FLAG(BOUNDARYCOMP) && (!((*pos)->FLAG(BODY))) && (!((*pos)->FLAG(ENDPOINT))) && (!((*pos)->FLAG(NOTCONNECTEDTOBODY))))
                    if (skel.IsCompartmentDeleteable26((*pos)) )
                    {
                        pos=eraseElement(pos);
                        if (pos!=List.begin()) pos--;
                        counter2++;
                    };
            cout << "Deleted in 18 Neigbourhood:" << counter2 << " comps." << endl;
	    
        }
	
	
	
        counter1=0;
        MarkBoundaryCompartments(26);
        for (pos=List.begin();pos!=List.end();++pos)
            if ( (*pos)->FLAG(BOUNDARYCOMP) && (!((*pos)->FLAG(BODY))) && (!((*pos)->FLAG(ENDPOINT))) && (!((*pos)->FLAG(NOTCONNECTEDTOBODY))))
                if (skel.IsCompartmentDeleteable26((*pos)) )
                {
                    pos=eraseElement(pos);
                    if (pos!=List.begin()) pos--;
                    counter1++;
                };
        cout << "Deleted in 26 Neigbourhood:" << counter1 << " comps." << endl;
	
    }
    
    
    
    callBackFunction(100);
    return 0;
};


/*!
* Searches for local maxima of distance in the compartments, checks 27 neigbourhood
 * these maxima should coresspondend with the dendrite terminations
 */
int
CompartmentList::
SearchLokalMaximaOfDistance27AndCreateButtonCompList()
{
    int dist;
    int counter=0, oldcounter=0;
    list<Compartment*>::iterator pos;
    short thereAreNotConnected=0;
    for (pos=List.begin();pos!=List.end();pos++)
    {
	
        counter++;
        if (counter>oldcounter+10)
        {
            oldcounter=counter;
            callBackFunction((100*counter)/NrOfCompartments);
        }
	
        if ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME)
        {
            
            thereAreNotConnected=1;
            (*pos)->CLEARFLAG(ENDPOINT);
            (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
	    
        }
        else
        {
            (*pos)->SETFLAG(ENDPOINT);
            dist=(*pos)->GetDistanceFromBody();
            int kind;
            for (kind=N_TopFront;kind<=N_Bottom;kind++)
                if ((*pos)->GetConnection(kind)!=NULL)
                    if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>dist)
                        (*pos)->CLEARFLAG(ENDPOINT);
	    
	    
            if ((*pos)->FLAG(ENDPOINT))
                buttonCompList.push_back((*pos));
        };
	
	
    }
    if (thereAreNotConnected) cout << "Threre are compartments which are not connected to the cellbody, they should be deleted!" << endl;
    cout << "Detected " << buttonCompList.size() << " dendrite terminations using a 27 neigbourhood (>)." << endl;
    return 0;
}




/*!
* Searches for local maxima of distance in the compartments, checks 27 neigbourhood
 * sharp detection, real maxium not only eqal or bigger, these maxima should coresspondend with the dendrite terminations
 */
int
CompartmentList::
SearchLokalMaximaOfDistance27SharpAndCreateButtonCompList()
{
    int dist;
    int counter=0;
    list<Compartment*>::iterator pos;
    short thereAreNotConnected=0;
    for (pos=List.begin();pos!=List.end();pos++)
    {
	
        counter++;
        callBackFunction((100*counter)/NrOfCompartments);
	
        if ((*pos)->GetDistanceFromBody()==MAXARRIVALTIME)
        {
            
            thereAreNotConnected=1;
            (*pos)->CLEARFLAG(ENDPOINT);
            (*pos)->SETFLAG(NOTCONNECTEDTOBODY);
            continue;
	    
        }
	
        (*pos)->SETFLAG(ENDPOINT);
        dist=(*pos)->GetDistanceFromBody();
        int kind;
        for (kind=N_TopFront;kind<=N_Bottom;kind++)
            if ((*pos)->GetConnection(kind)!=NULL)
                if (((*pos)->GetConnection(kind))->GetDistanceFromBody()>=dist)
                    (*pos)->CLEARFLAG(ENDPOINT);
	
	
        if ((*pos)->FLAG(ENDPOINT))
            buttonCompList.push_back((*pos));
	
	
	
    }
    if (thereAreNotConnected) cout << "Threre are compartments which are not connected to the cellbody, they should be deleted!" << endl;
    cout << "Detected " << buttonCompList.size() << " dendrite terminations using 27 neigbours and sharp distance (>=)." << endl;
    return 0;
}


/*!
* Searches for the other 21 neigbours, needs to know the 6 neigbourhood before! 
 */
int
CompartmentList::
SetNeigbours27()
{
    if (kindOfNeigbourHood>6)
    {
        cout << "List is already a " << kindOfNeigbourHood << " neigborhood list" << endl; 
        return 1;
    };
    list<Compartment*>::iterator pos;
    
    for (pos=List.begin();pos!=List.end();++pos)
        (*pos)->FindAndSetMyNeigbours27();
    kindOfNeigbourHood=26;
    
    return 0;
}


/*!
*This function Creates the neuron graph
 *It uses the distance field from the function CalcualteAndSetDistancesFromBodyCompartment() to detect the topological structure.
 *1. Walk from buttons to body - Detect Crossings and check if buttons are still connected
 *2. Create list of all crossings (called link list), create root link at body site
 *3. Walk from buttons to next proximal crossing, create Dendrite and connect the three structures, see nervecell.h
 *4. Walk from each link to next proximal link,  create Dendrite and connect the three structures, see nervecell.h
 */
int
CompartmentList::
WalkFromButtonsToBodyAndCreateNeuronStructure()
{
    list<Compartment*>::iterator pos;
    
    //recalculate the distances from body compartment
    ResetDistancesFromBody();
    CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction();
    
    
    Compartment *linkComp;
    Link *link;
    Dendrite *dendrite;
    Button *button;
    
    // 1. Walk from buttons to body - Detect Crossings and check if buttons are still connected
    
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
    {
        if ((*pos)->MarchToAndAddPassingThroughs(bodyComp)) {cout << "Colud not find way for Termination Comp:" << endl << **pos;return 1;};
        cout << "Create new button structure..." ;
        button=new Button();
        button->SetCompartment(*pos);
        (*pos)->SetStructure((Structure*) button);
        cout << "..done" << endl;
    };
    
    //Creates cellbody and connects him with coressponding compartment
    cout << "Create Cellbody!" <<endl;
    body=new Cellbody();
    body->SetCompartment(bodyComp);
    bodyComp->SetStructure((Structure*) body);
    
    
    // 2. Create list of all crossings (called link list), create root link at body site
    cout << "Create list for links" << endl;
    cout << "The cellbody compartent is connected to root link and to the cellbody structure" << endl;
    for (pos=List.begin();pos!=List.end();++pos)
        if ((*pos)->FLAG(BIFURCATION))
        {
            linkCompList.push_back(*pos);
            link=new Link();
            link->SetCompartment(*pos);
            (*pos)->SetLink(link);
        }
	    
	    // Walk from buttons to next proximal link (crossing), create Dendrite and connect the three structures, see nervecell.h
	    cout << "walk from button to nearest link and connect button via dendrite structure with link" << endl;
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
    {
        list<Compartment*> geometricComps;
        linkComp=(*pos)->MarchToNext(BIFURCATION, geometricComps);
        if (linkComp==NULL) {cout << "Colud not find way to for Comp:" << endl << **pos;return 1;};
        if ((*pos)->GetStructure()==NULL) {cout << "This button has no structure!:" << endl << **pos;return 1;};
        if (linkComp->GetLink()==NULL) {cout << "This linkComp has no link!:" << endl << *linkComp;return 1;};
	
        (linkComp->GetLink())->AddDistalStructure( (Structure*) (*pos)->GetStructure());
        ((*pos)->GetStructure())->SetProximalLink((linkComp->GetLink()));
        ((*pos)->GetStructure())->SetGeometricComps(geometricComps);
    };
    
    // 4. Walk from each link to next proximal link,  create Dendrite and connect the three structures, see nervecell.h
    cout << "walk from link to link and connect them via a dendrite structure" << endl;
    for (pos=linkCompList.begin();pos!=linkCompList.end();++pos)
    {
        if (*pos!=bodyComp)
        {
            if ((*pos)->GetLink()==NULL) {cout << "This LinkComp has no link!:" << endl << **pos;return 1;};
            Link *distalLink=(*pos)->GetLink();
            list<Compartment*> geometricComps;
            linkComp=(*pos)->MarchToNext(BIFURCATION,geometricComps); //here a whole list of comps should follow, for geometric reconstr.
            if (linkComp==NULL) {cout << "Colud not find way to Link for Comp:" << endl << **pos;return 1;};
            if (linkComp->GetLink()==NULL) {cout << "This linkComp has no link!:" << endl << *linkComp;return 1;};
            Link *proxLink=linkComp->GetLink();
            dendrite=new Dendrite();
            dendrite->SetProximalLink(proxLink);
            dendrite->SetDistalLink(distalLink);
            dendrite->SetGeometricComps(geometricComps);
            proxLink->AddDistalStructure((Structure*) dendrite);
            distalLink->SetProximalStructure((Structure*) dendrite);
        }
        else
        {
            cout << "Now connect cellbody structure with root link and vice versa" << endl;
            if ((*pos)->GetLink()==NULL) {cout << "This LinkComp has no link!:" << endl << **pos;return 1;};
            link=(*pos)->GetLink();
            link->SetProximalStructure((Structure*)body);
            body->SetDistalLink(link);
        }
    }
    
    cout << "Finished reconstruction" << endl;
    return 0;
}

//Auxillary functions for graph extraction


/*!
* deletes the compartments which are not connected to the cellbody compartment from the compartmentlist
 */
int
CompartmentList::
deleteNonConnected()
{
    list<Compartment*>::iterator pos;
    
    for (pos=List.begin();pos!=List.end();++pos)
    {
        if ( ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME) && (!(*pos)->FLAG(ENDPOINT)) )
        {
            pos=eraseElement(pos);
            if (pos!=List.begin()) pos--;
        };
	
        if ( ((*pos)->GetDistanceFromBody()>=MAXARRIVALTIME) && ((*pos)->FLAG(ENDPOINT)) )
        {
            cout << "Warning try to delete Endpoint Compartment!!!!" << endl;
        }
	
	
    };
    
    return 0;
};


/*!
* This functions resets the distance from cellbody compartment to the MAXARIVALTIME
 */
int
CompartmentList::
ResetDistancesFromBody()
{
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
        (*pos)->SetDistanceFromBody(MAXARRIVALTIME);
    
    return 0;
}


int
CompartmentList::
ResetNrOfPassingThroughAndBifurcationFlag()
{
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
    {
        (*pos)->SetNrOfPassingThrough(0);
        (*pos)->CLEARFLAG(BIFURCATION);
        (*pos)->SetNrOfLinkConnections(0);
    }
    return 0;
}



//Communication with GUI
/*! Creates Mark List with all detected terminations, this list can be modified 
*/
int
CompartmentList::
createMarkList(list<Mark*> &markList)
{
    list<Compartment*>::iterator pos;
    Mark *mark;
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
    {
	
        mark=new Mark((*pos)->GetStart(),aktive,(*pos)->GetID());
        //cout << "Member of Mark list with pos" << (*pos)->GetStart() << endl;
        markList.push_back(mark);
        
    };
    
    return 0;
}


/*! Add a termination to the Mark List  
*/
int
CompartmentList::
AddMark(Mark &newMark)
{
    //find compartment
    cout << "Try to add mark at position" << newMark.GetPosition() << endl;
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
        if ((*pos)-> IsPointInCompartment(newMark.GetPosition())) break;
    if (pos==List.end())
    {
        newMark.SetIdOfTermination(-1);
        return COULDNOTFINDTERMINATION;
    };
    
    //check if not already member of buton comp list
    if ((*pos)->FLAG(ENDPOINT))
    {
        newMark.SetIdOfTermination(-1);
        return THISCOMPARTMENTISALLREADYATERMINATION;
    };
    
    //check if the compartment is connected to cellbody
    if ((*pos)->FLAG(NOTCONNECTEDTOBODY))
    {
        newMark.SetIdOfTermination(-1);
        return THISCOMPARTMENTISNOTCONNECTED;
    };
    
    //add
    (*pos)->SETFLAG(ENDPOINT);
    buttonCompList.push_back((*pos));
    newMark.SetIdOfTermination((*pos)->GetID());
    return 0;
}

/*! Removes a mark and a termination from the list  
*/
int
CompartmentList::
DeleteMark(int compartmentId)
{
    list<Compartment*>::iterator pos;
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
        if ((**pos).GetID()==compartmentId)
	{
            (*pos)->CLEARFLAG(ENDPOINT);
            buttonCompList.erase(pos);
            return 0;
	};
    
    return COULDNOTFINDTERMINATION;
}


//update Button with modifications
int
CompartmentList::
updateButtonCompListWithMarkList(list<Mark*> &markList)
{
    //not needed anymore, list will be updated all the time!
    
    return 0;
}

//Use list for other opps


/*!
* To use the filter on big cubes you have to split him, therefore the octree approach is usefull
 */
int
CompartmentList::
filter(FilterOptions &fo)
{
    Vector<int> dV(3);
    Vector<int> start(3);
    Vector<int> size(3);
    Vector<int> pos(3);
    pos[1]=0;
    pos[2]=0;
    pos[3]=0;
    
    int d=6;
    dV[1]=d;
    dV[2]=d;
    dV[3]=d;
    DataCube target(*dataCube);
    NeuraDataType *P1=dataCube->GetDataPointer();
    NeuraDataType *P2=target.GetDataPointer();
    
    for(int z=0;z<dataCube->NrOfElements();z++)
        P2[z]=P1[z];
    
    
    int counter=0;
    
    
    list<Compartment*>::iterator posIt;
    for (posIt=List.begin();posIt!=List.end();++posIt)
    {
        counter++;
        cout << "Process with Cube: "<< counter << "of "<< NrOfCompartments << endl;
        callBackFunction((100*counter)/NrOfCompartments);
        DataCube source(3);
        start=(*posIt)->GetStart();
        size=(*posIt)->GetSize();
        size=size+2*dV;
        start=start-dV;
	
        cout << "Start Vector:"<< endl << start;
        cout << "Size Vector:" << endl << size;
	
        source.SetCubeSizeAndSpaceing(size,target.GetSpaceing());
        source.MallocCube();// can not do this outside because size of cubes are different
	    source.CopyDataCubeIntoDataCube(dataCube,start,size,pos);
	    NLD nld(&source); 
	    nld.SetFilterOptions(fo);
	    cout << "Start to filter..." ;
	    int error=nld.execute();
	    if (error) {cout <<"nld-test" ;return error;}
	    cout << "done." << endl;
	    target.CopyDataCubeIntoDataCube(&source,dV,(*posIt)->GetSize(),(*posIt)->GetStart());
	    source.freeMemory();
	    
    }
    for(int z=0;z<dataCube->NrOfElements();z++)
        P1[z]=P2[z];
    
    
    target.freeMemory();
    return 0;
    
}







//IO Functions
// There are loots of formats to write cubes down
// Most important, datacube and onom2k Format
//
//

int 
CompartmentList::
writeSlicesPPM(char* templateName, int sliceNr, int direction)
{
    char filename[1024];
    int i=0;
    
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
    {
	i++;
	sprintf(filename,"%sLevel:%dElement:%d.ppm",templateName,level,i);
	(*pos)->writeSlicePPM(filename,sliceNr,direction);
	
    };
    
    return 0;
};


const char headerTemplate[500] = 
"ONOM2KDATA\nGRIDTYPE UNSTRUCTURED_MIX\nSYMMETRY NONE\nSIZE %d %d\nDATANUMBER 4\nZ\nNAME location\nLABEL x\nMIN %d.0\nMAX %d.0\nZ\nNAME location\nLABEL y\nMIN %d.0\nMAX %d.0\nZ\nNAME location\nLABEL z\nMIN %d.0\nMAX %d.0\nZ\nNAME location\nLABEL c\nMIN 0.0\nMAX %d.0\nTEXT1 Compartmentlist\nTEXT2 \n";

const char positionTemplate[500] = "object \"position list\" class array type float rank 1 shape 3 items %d \ndata follows\n"; //%d number of positions

const char BeginEdgeTemplate[500] = "object \"edge list\" class array type int rank 0 items %d data follows\n"; //%d number of corners*4
const char EndEdgeTemplate[500] = "attribute \"ref\" string \"positions\" \n\n";

const char BeginLoopTemplate[500] = "object \"loop list\" class array type int rank 0 items %d data follows\n"; //%d number of edbes
const char EndLoopTemplate[500] = "\n\n";

const char BeginEdeobjTemplate[500] = "attribute \"ref\" string \"edges\"object \"face list\" class array type int rank 0 items %d data follows\n";// %d number of edges
const char EndEdeobjTemplate[500] = "attribute \"ref\" string \"loops\" \n\n";

const char BeginColorTemplate[500] = "object \"color list\" class array type float rank 1 shape 3 items %d data follows\n"; //%d number of edges, each edge own color
const char EndColorTemplate[500] = "attribute \"dep\" string \"faces\" \n\n"; 

const char BeginNormalTemplate[500] = " object \"normal list\" class array type float rank 1 shape 3 items %d \ndata follows\n"; //%d number of edges
const char EndNormalTemplate[500] = "attribute \"dep\" string \"faces\"\n\n";

const char ObjectDefinition[500] = "object \"cubus\" class field\ncomponent \"positions\"    \"position list\"\n component \"edges\"        \"edge list\"\n component \"loops\"   \"loop list\" \n component \"faces\"  \"face list\"\n component \"colors\"       \"color list\" \n component \"normals\"      \"normal list\"\nend\n";

int 
CompartmentList::
writeDX(char* Name)
{
    Vector<int> start=dataCube->GetStart();
    Vector<int> size =dataCube->GetSize();
    FILE *fptr;
    //char header[1000];
    char buffer[1000];
    //int maxcolor=4;
    if (Name == NULL)
	fptr = stdout;
    else
	fptr = fopen(Name, "w");
    if (fptr==NULL) return 1;
    
    sprintf(buffer,positionTemplate,NrOfCompartments*8);
    fprintf(fptr, buffer);
    double dx=  (dataCube->GetSpaceing())[1];
    double dy=  (dataCube->GetSpaceing())[2];
    double dz=  (dataCube->GetSpaceing())[3];
    
    
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
    {
	
	start=(*pos)->GetStart();
	size=(*pos)->GetSize();
	
	sprintf(buffer,"%f\t%f\t%f\n",start[1]*dx,start[2]*dy,start[3]*dz); //0
	fprintf(fptr,buffer);
	sprintf(buffer,"%f\t%f\t%f\n",(start[1]+size[1])*dx,start[2]*dy,start[3]*dz); //1
	fprintf(fptr,buffer);
	sprintf(buffer,"%f\t%f\t%f\n",(start[1]+size[1])*dx,(start[2]+size[2])*dy,start[3]*dz); //2
	fprintf(fptr,buffer);
	sprintf(buffer,"%f\t%f\t%f\n",start[1]*dx,(start[2]+size[2])*dy,start[3]*dz);//3
	    fprintf(fptr,buffer);
	    
	    sprintf(buffer,"%f\t%f\t%f\n",start[1]*dx,start[2]*dy,(start[3]+size[3])*dz);//4
		fprintf(fptr,buffer);
		sprintf(buffer,"%f\t%f\t%f\n",(start[1]+size[1])*dx,start[2]*dy,(start[3]+size[3])*dz);//5
		    fprintf(fptr,buffer);
		    sprintf(buffer,"%f\t%f\t%f\n",(start[1]+size[1])*dx,(start[2]+size[2])*dy,(start[3]+size[3])*dz);//6
			fprintf(fptr,buffer);
			sprintf(buffer,"%f\t%f\t%f\n",start[1]*dx,(start[2]+size[2])*dy,(start[3]+size[3])*dz);//7
			    fprintf(fptr,buffer);
			    
			    
			    
			    
			    
    };
    
    
    fprintf(fptr, "\n");  
    sprintf(buffer,BeginEdgeTemplate,NrOfCompartments*6*4);
    fprintf(fptr, buffer); 
    int j;
    for (int i=0;i<NrOfCompartments;i++)
    {
	j=i*8;
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+1,j+2,j+3); //deckel
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+4,j+5,j+1); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+5,j+6,j+2,j+1); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+7,j+6,j+2,j+3); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+4,j+5,j+6,j+7); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+4,j+7,j+3); 
	fprintf(fptr,buffer);
    }
    
    
    sprintf(buffer,EndEdgeTemplate);
    fprintf(fptr,buffer);
    
    
    
    sprintf(buffer,BeginLoopTemplate,NrOfCompartments*6);
    fprintf(fptr,buffer);
    for (int i=0;i<NrOfCompartments*6;i++)
    {
	j=i*4;
	sprintf(buffer,"%d \n",j);
	fprintf(fptr,buffer);
    }
    sprintf(buffer,EndLoopTemplate);
    fprintf(fptr,buffer);
    
    sprintf(buffer,BeginEdeobjTemplate,NrOfCompartments*6);
    fprintf(fptr,buffer);
    for (int i=0;i<NrOfCompartments*6;i++)
    {
	sprintf(buffer,"%d \n",i);
	fprintf(fptr,buffer);
    }
    sprintf(buffer,EndEdeobjTemplate);
    fprintf(fptr,buffer);
    
    sprintf(buffer,BeginColorTemplate,NrOfCompartments*6);
    fprintf(fptr,buffer);
    
    
    for (pos=List.begin();pos!=List.end();++pos)
    {
	if ((*pos)->FLAG(NOTCONNECTEDTOBODY))
	{
	    //white
	    sprintf(buffer,"0.0  0.0  0.0\n0.0  0.0  0.0\n0.0  0.0  0.0\n0.0  0.0  0.0\n0.0  0.0  0.0\n0.0  0.0  0.0\n");
	    fprintf(fptr,buffer);
	    continue;
	};
	
	if ((*pos)->FLAG(BODYELEMENT))
	{
	    //turquois
	    sprintf(buffer,"0.0  1.0  1.0\n0.0  1.0  1.0\n0.0  1.0  1.0\n0.0  1.0  1.0\n0.0  1.0  1.0\n0.0  1.0  1.0\n");
	    fprintf(fptr,buffer);
	    continue;
	};
	
	if ((*pos)->FLAG(BODY)) 
	{
	    //red
	    sprintf(buffer,"1.0  0.0  0.0\n1.0  0.0  0.0\n1.0  0.0  0.0\n1.0  0.0  0.0\n1.0  0.0  0.0\n1.0  0.0  0.0\n");
	    fprintf(fptr,buffer);
	    continue;
	};
	
	if ((*pos)->FLAG(BIFURCATION)) 
	{
	    //yellow
	    sprintf(buffer,"1.0  1.0  0.0\n1.0  1.0  0.0\n1.0  1.0  0.0\n1.0  1.0  0.0\n1.0  1.0  0.0\n1.0  1.0  0.0\n");
	    fprintf(fptr,buffer);
	    continue;
	};
	
	
	if ((*pos)->FLAG(ENDPOINT)) 
	{
	    //green
	    sprintf(buffer,"0.0  1.0  0.0\n0.0  1.0  0.0\n0.0  1.0  0.0\n0.0  1.0  0.0\n0.0  1.0  0.0\n0.0  1.0  0.0\n");
	    fprintf(fptr,buffer);
	    continue;
	};
	
	if ((*pos)->FLAG(BOUNDARYCOMP)) 
	{
	    //blue
	    sprintf(buffer,"0.0  0.0  1.0\n0.0  0.0  1.0\n0.0  0.0  1.0\n0.0  0.0  1.0\n0.0  0.0  1.0\n0.0  0.0  1.0\n");
	    fprintf(fptr,buffer);
	    continue;
	};
	
	sprintf(buffer,"0.6  0.3  0.6\n0.8  0.8  0.1\n0.9  0.4  0.9\n0.4  0.8  0.7\n0.8  0.8  0.8\n0.1  0.8  0.8\n");
	fprintf(fptr,buffer);
    } 
    
    
    sprintf(buffer,EndColorTemplate);
    fprintf(fptr,buffer);
    
    sprintf(buffer,BeginNormalTemplate,NrOfCompartments*6);
    fprintf(fptr,buffer);
    for (int i=0;i<NrOfCompartments;i++)
    {
	sprintf(buffer,"0.0   0.0 -1.0\n0.0  -1.0  0.0\n1.0   0.0  0.0\n0.0   1.0  0.0\n0.0   0.0  1.0\n-1.0   0.0  0.0\n");
	fprintf(fptr,buffer);
    }
    
    sprintf(buffer,EndNormalTemplate);
    fprintf(fptr,buffer);
    
    sprintf(buffer,ObjectDefinition);
    fprintf(fptr,buffer);
    
    fclose(fptr);
    return 0;
}





int 
CompartmentList::
writeOnom2K(char* Name)
{
    Vector<int> start=dataCube->GetStart();
    Vector<int> size =dataCube->GetSize();
    FILE *fptr;
    char header[1000];
    char buffer[300];
    int maxcolor=4;
    if (Name == NULL)
	fptr = stdout;
    else
	fptr = fopen(Name, "w");
    if (fptr==NULL) return 1;
    sprintf(header,headerTemplate, NrOfCompartments*8, NrOfCompartments*6,start[1],start[1]+size[1],start[2],start[2]+size[2],start[3],start[3]+size[3],maxcolor);
    fprintf(fptr, header);
    
    fprintf(fptr,"DATA\n");
    fprintf(fptr,"x\ty\tz\tc\n");
    int i=0;
    
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
    {
	i++;
	int color=i;
	start=(*pos)->GetStart();
	size=(*pos)->GetSize();
	--size[1];
	--size[2]; 
	--size[3];
	sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2],start[3],color); //0
	fprintf(fptr,buffer);
	sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2],start[3],color); //1
	fprintf(fptr,buffer);
	sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2]+size[2],start[3],color); //2
	fprintf(fptr,buffer);
	sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2]+size[2],start[3],color);//3
	    fprintf(fptr,buffer);
	    
	    sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2],start[3]+size[3],color);//4
		fprintf(fptr,buffer);
		sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2],start[3]+size[3],color);//5
		    fprintf(fptr,buffer);
		    sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1]+size[1],start[2]+size[2],start[3]+size[3],color);//6
			fprintf(fptr,buffer);
			sprintf(buffer,"%d.0\t%d.0\t%d.0\t%d.0\n",start[1],start[2]+size[2],start[3]+size[3],color);//7
			    fprintf(fptr,buffer);
    };
    fprintf(fptr,"ELEMENTS\n");
    int j;
    for (int i=0;i<NrOfCompartments;i++)
    {
	j=i*8;
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+1,j+2,j+3); //top
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+4,j+5,j+1); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+5,j+6,j+2,j+1); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+7,j+6,j+2,j+3); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+4,j+5,j+6,j+7); 
	fprintf(fptr,buffer);
	sprintf(buffer,"%d\t%d\t%d\t%d\n",j+0,j+4,j+7,j+3); 
	fprintf(fptr,buffer);
    }
    
    fprintf(fptr,"END\n");
    fclose(fptr);
    return 0;
}


int
CompartmentList::
writeToDataCube(DataCube *target)
{
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
        (*pos)->writeToDataCube(target);
    return 0;
};



int
CompartmentList::
writeButtonCompListToDataCube(DataCube *target)
{
    list<Compartment*>::iterator pos;
    for (pos=buttonCompList.begin();pos!=buttonCompList.end();++pos)
        (*pos)->writeToDataCube(target);
    return 0;
};



int 
CompartmentList::
TestOutPut()
{
    Skeletonization skel;
    Primitive *prim; 
    int nr=0;
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
	if ((*pos)->GetNrOfNeigbours()<2)
	{
	    char buffer[80];
	    nr++;
	    prim=skel.CompartmentToPrimitive(*pos);
	    sprintf(buffer,"tmp/singleneigbourId%dNr%d.onom",(*pos)->GetID(),nr);
	    prim->writeOnom2K(buffer);
	    cout << *(*pos);
	    cout << "Skel Returns:" << skel.IsPrimitivetDeleteable(prim) << endl;
	    
	}
	    return 0;
    
}   

int
CompartmentList::
PrintCompartmentWithId(int id)
{
    list<Compartment*>::iterator pos;
    for (pos=List.begin();pos!=List.end();++pos)
	if ((*pos)->GetID()==id)
	{
            cout << (**pos) << endl;
            return 0;
	};
    cout << "Can not find any compartment with id" << id << endl;
    return 1;
}

ostream &operator<<(ostream &ostr, const CompartmentList &CL)
{
    
    ostr << "*****CompartmentList*****"<< endl;
    ostr << "ID:" << CL.id << endl;
    ostr << "Nr of compartments:" << CL.NrOfCompartments << endl;
    ostr << "First Element:" << endl;	
    ostr << **(CL.List.begin()) << endl;
    ostr << "*************************"<< endl;
    return ostr;
};



