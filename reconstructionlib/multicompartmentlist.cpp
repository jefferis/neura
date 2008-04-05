#include "multicompartmentlist.h"


// Full functional methods


static int
EmptyCallBackFunction(int percentage)
{
    return 0;
};


//Constructor and destructor
MultiCompartmentList::
MultiCompartmentList(DataCube *dC,int (*callBack) (int percentage))
{
    dataCube=dC;
    CompartmentList *CL;
    presentlevel=1;
    maxlevel=1;
    CL=new CompartmentList(dataCube,presentlevel,callBack);
    if (CL==NULL)
    {
        cout << "Cant malloc!\n" << endl;
        exit(1);
    };
    compartmentList.push_front(CL);
    Compartment *comp;
    comp= new Compartment(dataCube,0,dataCube->GetStart(),dataCube->GetSize(),NULL,ORIGIN);
    CL->addElementFront(comp);
    bodyComp=NULL;
    callBackFunction=callBack;
}



MultiCompartmentList::
MultiCompartmentList(DataCube *dC)
{
  dataCube=dC;
  CompartmentList *CL;
  presentlevel=1;
  maxlevel=1;
  CL=new CompartmentList(dataCube,presentlevel);
  if (CL==NULL) 
    {
      cout << "Cant malloc!\n" << endl;
      exit(1);
    };
  compartmentList.push_front(CL);
  Compartment *comp;
  comp= new Compartment(dataCube,0,dataCube->GetStart(),dataCube->GetSize(),NULL,ORIGIN);
  CL->addElementFront(comp);
  bodyComp=NULL;
  callBackFunction=EmptyCallBackFunction;
}


MultiCompartmentList::
~MultiCompartmentList()
{
 
  list<CompartmentList*>::iterator pos;
  for (pos=compartmentList.begin();pos!=compartmentList.end();pos++)
    {
      delete *pos;
    }

}




Compartment*
MultiCompartmentList::
DirektAcessCompartment(Compartment** field, int posX, int posY, int posZ, short dX, short dY, short dZ)
{
    int aX=posX+dX;
    int aY=posY+dY;
    int aZ=posZ+dZ;

    if (aX<0 || aX>=(dataCube->GetSize())[1]) return NULL;
    if (aY<0 || aY>=(dataCube->GetSize())[2]) return NULL;
    if (aZ<0 || aZ>=(dataCube->GetSize())[3]) return NULL;
    return field[aX+aY*(dataCube->GetSize())[1]+aZ*(dataCube->GetSize())[1]*(dataCube->GetSize())[2]];
}

Compartment*
MultiCompartmentList::
DirectNeigbourAccess(Compartment** field, int posX, int posY, int posZ, int neigbour)
{
    switch (neigbour)
    {
	//top
        case  N_TopFront      : return DirektAcessCompartment(field,posX,posY,posZ, Middle ,Front , Top );break;
        case  N_TopBack       : return DirektAcessCompartment(field,posX,posY,posZ, Middle, Back, Top);break;
        case  N_TopFrontRight : return DirektAcessCompartment(field,posX,posY,posZ, Right, Front , Top);break;
        case  N_TopRight      : return DirektAcessCompartment(field,posX,posY,posZ, Right, Middle, Top);break;
        case  N_TopLeft       : return DirektAcessCompartment(field,posX,posY,posZ, Left, Middle, Top);break;
        case  N_TopFrontLeft  : return DirektAcessCompartment(field,posX,posY,posZ, Left, Front, Top);break;
        case  N_TopBackLeft   : return DirektAcessCompartment(field,posX,posY,posZ, Left, Back, Top);break;
        case  N_TopBackRight  : return DirektAcessCompartment(field,posX,posY,posZ, Right ,Back , Top);break;
        case  N_Top           : return DirektAcessCompartment(field,posX,posY,posZ, Middle , Middle, Top);break;
	//samelevel
        case  N_Right         : return DirektAcessCompartment(field,posX,posY,posZ, Right, Middle , Middle);break;
        case  N_FrontRight    : return DirektAcessCompartment(field,posX,posY,posZ, Right, Front, Middle);break;
        case  N_Back          : return DirektAcessCompartment(field,posX,posY,posZ, Middle, Back, Middle);break;
        case  N_Left          : return DirektAcessCompartment(field,posX,posY,posZ, Left, Middle , Middle);break;
        case  N_FrontLeft     : return DirektAcessCompartment(field,posX,posY,posZ, Left, Front , Middle);break;
        case  N_BackLeft      : return DirektAcessCompartment(field,posX,posY,posZ, Left, Back, Middle);break;
        case  N_BackRight     : return DirektAcessCompartment(field,posX,posY,posZ, Right, Back, Middle);break;
        case  N_Front         : return DirektAcessCompartment(field,posX,posY,posZ, Middle, Front, Middle);break;
	//bottom
        case  N_BottomRight    	 : return DirektAcessCompartment(field,posX,posY,posZ, Right,Middle , Bottom);break;
        case  N_BottomFrontRight : return DirektAcessCompartment(field,posX,posY,posZ, Right, Front, Bottom );break;
        case  N_BottomBack  	 : return DirektAcessCompartment(field,posX,posY,posZ, Middle, Back, Bottom);break;
        case  N_BottomLeft  	 : return DirektAcessCompartment(field,posX,posY,posZ, Left, Middle, Bottom);break;
        case  N_BottomFrontLeft  : return DirektAcessCompartment(field,posX,posY,posZ, Left, Front, Bottom);break;
        case  N_BottomBackLeft   : return DirektAcessCompartment(field,posX,posY,posZ, Left,Back , Bottom);break;
        case  N_BottomBackRight  : return DirektAcessCompartment(field,posX,posY,posZ, Right, Back, Bottom);break;
        case  N_BottomFront      : return DirektAcessCompartment(field,posX,posY,posZ, Middle, Front, Bottom);break;
        case  N_Bottom   	 : return DirektAcessCompartment(field,posX,posY,posZ, Middle, Middle, Bottom);break;
    };

    return NULL;
}




int
MultiCompartmentList::
createFinestCompartmentListFromScratch()
{
    CompartmentList* fineList;
    Compartment *comp, *neigbour;

    Compartment **field;
    field=(Compartment**) malloc (sizeof(Compartment*)*(dataCube->NrOfElements()));
    for (int n=0;n<dataCube->NrOfElements();n++) field[n]=NULL;

    Vector<int> size(3);
    Vector<int> start(3);
    size[1]=1;
    size[2]=1;
    size[3]=1;
    double threshold=0.1;
    maxlevel++;
    presentlevel=maxlevel;
    fineList= new CompartmentList(dataCube,maxlevel,callBackFunction);
    if (fineList==NULL)
    {
        cout << "Cant malloc!\n" << endl;
        exit(1);
    };
    compartmentList.push_back(fineList);

    int counter=0;
    //Create compartents, one compartment = one voxel
    cout << "Create compartments" << endl << flush;
    for(int z=0;z<(dataCube->GetSize())[3];z++)
        for(int y=0;y<(dataCube->GetSize())[2];y++)
            for(int x=0;x<(dataCube->GetSize())[1];x++)
                if ((*dataCube)(x,y,z)>threshold)
                {
                    start[1]=x;
                    start[2]=y;
                    start[3]=z;
                    comp= new Compartment(dataCube,presentlevel,start,size,NULL,ORIGIN);
                    if (comp==NULL)
                    {
                        cerr << "multicompartmentlist: create from scratch : Can not malloc quit!" << endl;
                        return 1;
                    };
                    fineList->addElementFront(comp);
                    field[x+y*(dataCube->GetSize())[1]+z*(dataCube->GetSize())[1]*(dataCube->GetSize())[2]]=comp;
                    counter++;

                };

    cout << "Created: " << counter << " compartments" << endl << flush;

    //Set neigbours
    cout << "Set neigbours" << endl << flush;
    for(int z=0;z<(dataCube->GetSize())[3];z++)
        for(int y=0;y<(dataCube->GetSize())[2];y++)
            for(int x=0;x<(dataCube->GetSize())[1];x++)
            {
                comp=field[x+y*(dataCube->GetSize())[1]+z*(dataCube->GetSize())[1]*(dataCube->GetSize())[2]];
                if (comp!=NULL)
                {
                    for (int nrOfNeigbour=N_TopFront;nrOfNeigbour<=N_Bottom;nrOfNeigbour++)
                    {
                        neigbour=DirectNeigbourAccess(field, x, y, z,nrOfNeigbour);
                        if (neigbour!=NULL)
                        {
                            comp->SetNeigbour(neigbour,nrOfNeigbour);
                        };
                    };
                };
            };
    

    fineList->SetKindOfNeigbourHood(26);
    cout << (*fineList) << endl;
    //free(field);
    return 0;

}





//functions to cretae hieracy
int 
MultiCompartmentList::
createNextLevel()
{
  CompartmentList* coarseList;
  CompartmentList* fineList;
  list<Compartment*>::iterator pos;
  int nr;
  Compartment **field;
 
  if (compartmentList.empty()) {cout << "List is empty!" << endl;return 1;}
  coarseList= compartmentList.back();
  maxlevel++;
  fineList= new CompartmentList(dataCube,maxlevel,callBackFunction);
  if ((coarseList->GetList()).empty()) {cout << "Coarse List is empty!" << endl;return 1;};
 

  for (pos=(coarseList->GetList()).begin();pos!=(coarseList->GetList()).end();++pos)
    {
      
      splitCompartment(*pos,&field,nr);
      for (int i=0;i<nr;i++)
	      {
	         fineList->addElementFront(field[i]);
	      };
      
      free(field);
    }
    
  fineList->EvaluateAndSetNeigboursForList();
  compartmentList.push_back(fineList);

  return 0;
}

//functions to cretae hieracy
int 
MultiCompartmentList::
createNextLevelSmart()
{
    CompartmentList* coarseList;
    CompartmentList* fineList;
    Compartment* firstCompartment;
    Vector<int> size(3);
    Vector<int> start(3);
    int maxsize;
    
    list<Compartment*>::iterator pos;
    int nr;
    Compartment **field;
    
    if (compartmentList.empty()) {cout << "List is empty!" << endl;return 1;}
    coarseList= compartmentList.back();
    maxlevel++;
    fineList= new CompartmentList(dataCube,maxlevel,callBackFunction);
    if ((coarseList->GetList()).empty()) {cout << "Coarse List is empty!" << endl;return 1;};
    
    firstCompartment=*((coarseList->GetList()).begin());
    size=firstCompartment->GetSize();
    size[1];
    size[2];
    size[3];
	
    
    
    for (pos=(coarseList->GetList()).begin();pos!=(coarseList->GetList()).end();++pos)
    {
	if (size[1]>2*size[3] || size[2]>2*size[3]) {nr=4;} else nr=0;
	splitCompartment(*pos,&field,nr);
	for (int i=0;i<nr;i++)
	{
	    fineList->addElementFront(field[i]);
	};
	
	free(field);
    }
    
    //no neigbours, only for filtering!!! fineList->EvaluateAndSetNeigboursForList();
    compartmentList.push_back(fineList);
    firstCompartment=*((fineList->GetList()).begin());
    size=firstCompartment->GetSize();
    maxsize=size[1];
    if (size[2]>maxsize) maxsize=size[2];
    if (size[3]>maxsize) maxsize=size[3];
    return maxsize;
}









//Graph extraction
int
MultiCompartmentList::
SetBody(Vector<int> position)
{
    Compartment *comp;
    cout << "Search for finest compartment surrounding body at : " << endl << position << endl;
    comp=FindFinestCompartmentSurrounding(position);
    if (comp==NULL)
    {
        cerr << "Cant find Origin!!!" <<endl;
        return POSITIONOFCELLBODYNOTVALID;
    };

    if (comp->GetLevel()<maxlevel)
    {
        cerr << "Does not belong to maxlevel="<<  maxlevel  << endl;
        cerr << "Level of Comp is:" << comp->GetLevel() << endl;
        return POSITIONOFCELLBODYNOTVALID;


    }
    cout << "Body Compartment:" << endl;
    cout << (*comp) << endl;
    comp->SETFLAG(BODY);
    bodyComp=comp;
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SetBody(comp);
    return 0;
}


/*!
* If you use no hierachy you must use this function to detect cellbody
*/
int
MultiCompartmentList::
SetBodyDirectFromFinestList(Vector<int> position)
{
    Compartment *comp;
    cout << "Search for finest compartment surrounding body at : " << endl << position << endl;
    comp= FindFinestCompartmentSurroundingInFinestList(position);
    if (comp==NULL)
    {
        cerr << "Cant find Origin!!!" <<endl;
        return POSITIONOFCELLBODYNOTVALID;
    };

    if (comp->GetLevel()<maxlevel)
    {
        cerr << "Does not belong to maxlevel="<<  maxlevel  << endl;
        cerr << "Level of Comp is:" << comp->GetLevel() << endl;
        return POSITIONOFCELLBODYNOTVALID;


    }
    cout << "Body Compartment:" << endl;
    cout << (*comp) << endl;
    comp->SETFLAG(BODY);
    bodyComp=comp;
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SetBody(comp);
    return 0;
}





//
// All these function just call the same routine of the finest list
// see compartmentlist.cpp for a description
//

int 
MultiCompartmentList::
ReduceFinestList(double epsilon)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->reduceList(epsilon);
    return 0;
}

int 
MultiCompartmentList::
DeleteNonConnectedFromFinestList()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->deleteNonConnected();
    return 0;
}



//Use hierachy for other operations
int
MultiCompartmentList::
filter(FilterOptions &fo)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->filter(fo);
    
}



/*!
* This calcuates the distance from the body compartment 6neigbourhood. Therefore it assigns each comp a distance from the origin.
 */
int
MultiCompartmentList::
CalcualteAndSetDistancesFromBodyCompartment()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->CalcualteAndSetDistancesFromBodyCompartment();
    
}



/////////////////////////////    Shell model      //////////////////////////////////////////////////////
/*
* This calculates the distance from the body compartment with the shell model.
*/

int   
MultiCompartmentList::
CalcualteAndSetDistancesFromBodyCompartment_ShellModel()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->CalcualteAndSetDistancesFromBodyCompartment_shellModel();
    compList->CalcualteAndSetIndex_shellModel();
	return 0;
}

/*!
* Searches for dendrite terminations
*/
int
MultiCompartmentList::
SearchLokalMaximaAndCreateButtonCompList_ShellModel()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SearchLokalMaximaAndCreateButtonCompList_ShellModel();

    return 0;
}


int
MultiCompartmentList::
CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther_ShellModel(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther_ShellModel(minDistanceFromBodyCompartment,minLengthOfDendriteInqm);
    return 0;
}

int 
MultiCompartmentList::
DetectCellbody(list<Vector<double> > *cellbodyList, double percentage)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->DetectCellbody(cellbodyList, percentage);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////





/*!
* This calcuates the distance from the body compartment 27neigborhood. Therefore it assigns each comp a distance from the origin.
 */

int   
MultiCompartmentList::
CalcualteAndSetDistancesFromBodyCompartment27()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->CalcualteAndSetDistancesFromBodyCompartment27();
    
}


int
MultiCompartmentList::
CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction();
    
}

/*!
* Searches for dendrite terminations
*/
int
MultiCompartmentList::
SearchLokalMaximaOfDistance6AndCreateButtonCompList()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SearchLokalMaximaOfDistance6AndCreateButtonCompList();

    return 0;
}

int
MultiCompartmentList::
CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther(minDistanceFromBodyCompartment,minLengthOfDendriteInqm);
    return 0;
}




/*!
* After creation of multicomplist this functions marks boundary comps 
*/
int
MultiCompartmentList::
MarkBoundaryCompartments()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList-> MarkBoundaryCompartments();
    return 0;

}

int
MultiCompartmentList::
ExtractSkeleton()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->extractSkeleton();
    return 0;
}

int
MultiCompartmentList::
ExtractSkeleton2()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->extractSkeleton2();
    return 0;
}

int
MultiCompartmentList::
SetNeigbour27()
{
    //int dist;
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SetNeigbours27();

    return 0;
}



int
MultiCompartmentList::
WalkFromButtonsToBodyAndCreateNeuronStructure()
{
    //int dist;
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->WalkFromButtonsToBodyAndCreateNeuronStructure();
}


int
MultiCompartmentList::
SearchLokalMaximaOfDistance27AndCreateButtonCompList()
{
    //int dist;
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SearchLokalMaximaOfDistance27AndCreateButtonCompList();

    return 0;
}


int
MultiCompartmentList::
SearchLokalMaximaOfDistance27SharpAndCreateButtonCompList()
{
  //  int dist;
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->SearchLokalMaximaOfDistance27SharpAndCreateButtonCompList();

    return 0;
}


//IO functions
int
MultiCompartmentList::
writeSlicesPPM(int level,char* templateName, int sliceNr, int direction)
{
    int i=1;
    list<CompartmentList*>::iterator pos;
    for (pos=compartmentList.begin();pos!=compartmentList.end();++pos)
        if (level==i) break;

    if (pos==compartmentList.end()) return 1;
    (*pos)->writeSlicesPPM(templateName,sliceNr,direction);
    return 0;

}



int
MultiCompartmentList::
writeOnom2K(int level,char* Name)
{

    CompartmentList* compList;
    compList=compartmentList.back();
    compList->writeOnom2K(Name);
    return 0;

}


int
MultiCompartmentList::
writeDX(int level,char* Name)
{

    CompartmentList* compList;
    compList=compartmentList.back();
    compList->writeDX(Name);
    return 0;

}

int
MultiCompartmentList::
createMarkList(list<Mark*> &markList)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->createMarkList(markList);
}

int
MultiCompartmentList::
AddMark(Mark &newMark)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->AddMark(newMark);
}


int
MultiCompartmentList::
DeleteMark(int id)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->DeleteMark(id);

}

int
MultiCompartmentList::
writeToDataCube(DataCube *target)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->writeToDataCube(target);
    return 0;
}

int
MultiCompartmentList::
writeButtonCompListToDataCube(DataCube *target)
{
    CompartmentList* compList;
    compList=compartmentList.back();
    compList->writeButtonCompListToDataCube(target);
    return 0;
}

int
MultiCompartmentList::
writeTestOutPut()
{

    CompartmentList* compList;
    compList=compartmentList.back();
    compList->MarkBoundaryCompartments();
    compList->TestOutPut();
    return 0;

}


//Data access
Cellbody*
MultiCompartmentList::
GetCellBody()
{
    CompartmentList* compList;
    compList=compartmentList.back();
    return compList->GetCellBody();
}



ostream &operator<<(ostream &ostr, const MultiCompartmentList &MCL)
{
    ostr << "#####MultiCompartemtList#####"<< endl;
    int i=0;
    list<CompartmentList*>::const_iterator pos;
    for (pos=MCL.compartmentList.begin();pos!=MCL.compartmentList.end();++pos)
    {
	ostr << "Level:" << ++i << endl;
	ostr << **pos << endl;
    }
    ostr << "#############################"<< endl;
    return ostr;
}


// from here on Private
// again full functional methods
//
// methods to search for the body compartment


/*
 * This functions search a compartment which surrounds a given position.
 */
Compartment*
MultiCompartmentList::
FindFinestCompartmentSurrounding(Compartment *comp,Vector<int> position)
{

    for (short kind=SONBASENR;kind<NROFCHILDKINDS;kind++)
    {
        if ( (comp->GetConnection(kind)) !=NULL)
            if ( (comp->GetConnection(kind)) ->IsPointInCompartment(position))
            {
                cout << "Is in Comp!!!" << endl;
                return FindFinestCompartmentSurrounding( (comp->GetConnection(kind)),position);
            }
    }

        return comp;

}


Compartment*
MultiCompartmentList::
FindFinestCompartmentSurrounding(Vector<int> position)
{

    if (compartmentList.empty()) {cout << "List is empty!" << endl;return NULL;}

    CompartmentList* compList;
    compList= compartmentList.front();
    list<Compartment*>::iterator pos;

    for (pos=(compList->GetList()).begin();pos!=(compList->GetList()).end();++pos)
        if ((*pos)-> IsPointInCompartment(position)) break;
    cout << "Found compartment surrounding" << position << endl << *pos << endl;
    if (pos==(compList->GetList()).end() )
    {
        cout << "Did not find any compartments surrounding this position" << endl;
        return NULL;
    }
    return FindFinestCompartmentSurrounding(*pos,position);

}





/*!
 * Searchs for a compartment surrounding a given position just looking at the finest 
 * compartment list.
 *
 */
Compartment*
MultiCompartmentList::
FindFinestCompartmentSurroundingInFinestList(Vector<int> position)
{

    if (compartmentList.empty()) {cout << "List is empty!" << endl;return NULL;}

    CompartmentList* compList;
    compList= compartmentList.back();
    list<Compartment*>::iterator pos;
    for (pos=(compList->GetList()).begin();pos!=(compList->GetList()).end();++pos)
        if ((*pos)-> IsPointInCompartment(position)) break;
    cout << "Found compartment surrounding" << position << endl << *pos << endl;
   
    if (pos==(compList->GetList()).end() )
    {
        cout << "Did not find any compartments surrounding this position" << endl;
        return NULL;
    }
    return *pos;

}

//methods to create next level of octree

/*!
*  Splits a compartment into 4 of the same size, size x,y are divided by 2 z stays unchanged
*/
int
MultiCompartmentList::
splitCompartment2D(Compartment *comp,Compartment ***field,int& nr)
{
    nr=4;
    Vector<int> start=comp->GetStart();
    Vector<int> size=comp->GetSize();
    int A1, A2, B1, B2;

    A1=size[1]/2;
    A2=size[1]-A1;
    B1=size[2]/2;
    B2=size[2]-B1;

    /*Compartment1*/
    Vector<int> start1 = comp->GetStart();
    Vector<int> size1  = comp->GetSize();
    size1[1]=A1;
    size1[2]=B1;

    /*Compartment2*/
    Vector<int> start2 = comp->GetStart();
    Vector<int> size2  = comp->GetSize();
    start2[1]+=A1;
    size2[1]=A2;
    size2[2]=B1;

    /*Compartment3*/
    Vector<int> start3 = comp->GetStart();
    Vector<int> size3  = comp->GetSize();
    start3[2]+=B1;
    size3[1]=A1;
    size3[2]=B2;

    /*Compartment4*/
    Vector<int> start4 = comp->GetStart();
    Vector<int> size4  = comp->GetSize();
    start4[1]+=A1;
    start4[2]+=B1;
    size4[1]=A2;
    size4[2]=B2;

    *field = (Compartment**) malloc (sizeof(Compartment*)*4);
    (*field)[0]=new Compartment(dataCube,maxlevel,start1,size1,comp,S_TopLeftFront);
    (*field)[1]=new Compartment(dataCube,maxlevel,start2,size2,comp,S_TopRightFront);
    (*field)[2]=new Compartment(dataCube,maxlevel,start3,size3,comp,S_TopLeftRear);
    (*field)[3]=new Compartment(dataCube,maxlevel,start4,size4,comp,S_TopRightRear);



    return 0;
}


/*!
*  Splits a compartment into 4 of the same size (size x,y,z divided by 2)
*/
int
MultiCompartmentList::
splitCompartment3D(Compartment *comp,Compartment ***field,int& nr)
{
    nr=8;
    Vector<int> start=comp->GetStart();
    Vector<int> size=comp->GetSize();
    int A1, A2, B1, B2, C1, C2;

    A1=size[1]/2;
    A2=size[1]-A1;
    B1=size[2]/2;
    B2=size[2]-B1;
    C1=size[3]/2;
    C2=size[3]-C1;

    /*Compartment1 S_TopLeftFront */
    Vector<int> start1 = comp->GetStart();
    Vector<int> size1  = comp->GetSize();
    size1[1]=A1;
    size1[2]=B1;
    size1[3]=C1;

    /*Compartment2 S_TopRightFront */
    Vector<int> start2 = comp->GetStart();
    Vector<int> size2  = comp->GetSize();
    start2[1]+=A1;
    size2[1]=A2;
    size2[2]=B1;
    size2[3]=C1;

    /*Compartment3  S_TopLeftRear  */
    Vector<int> start3 = comp->GetStart();
    Vector<int> size3  = comp->GetSize();
    start3[2]+=B1;
    size3[1]=A1;
    size3[2]=B2;
    size3[3]=C1;


    /*Compartment4   TopRightRear */
    Vector<int> start4 = comp->GetStart();
    Vector<int> size4  = comp->GetSize();
    start4[1]+=A1;
    start4[2]+=B1;
    size4[1]=A2;
    size4[2]=B2;
    size4[3]=C1;


    /*Compartment5   S_BottomLeftFront  */
    Vector<int> start5 = comp->GetStart();
    Vector<int> size5  = comp->GetSize();
    start5[3]+=C1;
    size5[1]=A1;
    size5[2]=B1;
    size5[3]=C2;

    /*Compartment  S_BottomRightFront  6*/
    Vector<int> start6 = comp->GetStart();
    Vector<int> size6  = comp->GetSize();
    start6[3]+=C1;
    start6[1]+=A1;

    size6[1]=A2;
    size6[2]=B1;
    size6[3]=C2;

    /*Compartment  S_BottomLeftRear  7*/
    Vector<int> start7 = comp->GetStart();
    Vector<int> size7  = comp->GetSize();
    start7[3]+=C1;
    start7[2]+=B1;
    size7[1]=A1;
    size7[2]=B2;
    size7[3]=C2;


    /*Compartment S_BottomRightRear  8*/
    Vector<int> start8 = comp->GetStart();
    Vector<int> size8  = comp->GetSize();
    start8[3]+=C1;
    start8[1]+=A1;
    start8[2]+=B1;
    size8[1]=A2;
    size8[2]=B2;
    size8[3]=C2;

    *field = (Compartment**) malloc (sizeof(Compartment*)*nr);
    (*field)[0]=new Compartment(dataCube,maxlevel,start1,size1,comp, S_TopLeftFront);
    (*field)[1]=new Compartment(dataCube,maxlevel,start2,size2,comp, S_TopRightFront);
    (*field)[2]=new Compartment(dataCube,maxlevel,start3,size3,comp, S_TopLeftRear);
    (*field)[3]=new Compartment(dataCube,maxlevel,start4,size4,comp, S_TopRightRear);
    (*field)[4]=new Compartment(dataCube,maxlevel,start5,size5,comp, S_BottomLeftFront);
    (*field)[5]=new Compartment(dataCube,maxlevel,start6,size6,comp, S_BottomRightFront);
    (*field)[6]=new Compartment(dataCube,maxlevel,start7,size7,comp, S_BottomLeftRear);
    (*field)[7]=new Compartment(dataCube,maxlevel,start8,size8,comp, S_BottomRightRear);
    for (int i=0;i<8;i++)
        comp->SetSon( (*field)[i], i);

    //think, must the var field be freed?
    return 0;
}


/*
 * Switches between the different kind of spliting possibilities
 *
 */
int
MultiCompartmentList::
splitCompartment(Compartment  *comp,Compartment ***field,int& nr)
{
    if (dataCube->GetDim()==3 && nr==4)
        return splitCompartment2D(comp,field,nr);
    
    if (dataCube->GetDim()==2)
        return splitCompartment2D(comp,field,nr);
    
    if (dataCube->GetDim()==3)
        return splitCompartment3D(comp,field,nr);

    return 1;
}

int
MultiCompartmentList::
singleChildCompartment(Compartment  *comp,Compartment **singleChild)
{
    *singleChild=new Compartment(dataCube,maxlevel,comp->GetStart(),comp->GetSize(),comp,S_SingleChild);
    return 0;
};



