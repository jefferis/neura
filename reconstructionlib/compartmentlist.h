

/****************************************************************************/
/*                                                                          */
/* File:      compatmentlist.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for data handling for reconstruction         */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*                                                                          */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   29.11.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef COMPARTMENTLIST_H
#define COMPARTMENTLIST_H

#include <stdio.h>

#include <iostream>


#include <list>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"
#include "../mathlib/transform.h"

#include "../mathlib/nld.h"

#include "../errorcode.h"

#include "compartment.h"
#include "skeletonization.h"
#include "nervecell.h"
#include "mark.h"




using namespace std;

class CompartmentList
{
public:
    CompartmentList(DataCube *dC,int Llevel);
    CompartmentList(DataCube *dC,int Llevel,int (*callBack) (int percentage));
    ~CompartmentList();

    //Modify List of Compartments
    int addElementFront(Compartment  *comp);
    int addElementEnd(Compartment *comp);
    int insertElement(list<Compartment*>::iterator pos,Compartment *comp);
    int deleteElement(list<Compartment*>::iterator pos);
    list<Compartment*>::iterator eraseElement(list<Compartment*>::iterator pos);
    int reduceList(double epsilon);

    //Create Neigbourhood
    int SetKindOfNeigbourHood(int kind) {kindOfNeigbourHood=kind;return 0;}
    int EvaluateAndSetNeigboursForList();

    //Graph extraction
    int SetBody( Compartment *comp) {bodyComp=comp;return 0;};
    int CalcualteAndSetDistancesFromBodyCompartment();
    int CalcualteAndSetDistancesFromBodyCompartment27();
    int CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction();
    int SearchLokalMaximaOfDistance6AndCreateButtonCompList();
    int CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm);
    int MarkBoundaryCompartments();
    int MarkBoundaryCompartments(short neigbourhoodType);
    int extractSkeleton();
    int extractSkeleton2();
    int SearchLokalMaximaOfDistance27AndCreateButtonCompList();
    int SearchLokalMaximaOfDistance27SharpAndCreateButtonCompList();
    int SetNeigbours27();
    int WalkFromButtonsToBodyAndCreateNeuronStructure();


///////////////////////////////////////////////////////////////////////////////
///   shell model 
//////////////////////////////////////////////////////////////////////////////

    int shelModelDistance(int kind);
    int CalcualteAndSetDistancesFromBodyCompartment_shellModel();
    int CalcualteAndSetIndex_shellModel();
    list<Compartment*> SetNeighbourShellIndex(list<Compartment*> frontList, int i);
    int arrangeList();
    int SearchLokalMaximaAndCreateButtonCompList_ShellModel();
    int CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther_ShellModel(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm);
	int DetectCellbody(list<Vector<double> > *cellbodyList, double percentage);


//////////////////////////////////////////////////////////////////////////////


    //Auxillary functions for graph extraction
    int deleteNonConnected();
    int ResetDistancesFromBody();
    int ResetNrOfPassingThroughAndBifurcationFlag();

    //Communication with GUI
    //Create Mark List
    int createMarkList(list<Mark*> &markList);
    int AddMark(Mark &newMark);
    int DeleteMark(int id);
    
    //update Button with modifications
    int updateButtonCompListWithMarkList(list<Mark*> &markList);
    

    
    //Use list for other opps
    int filter(FilterOptions &fo);


    //IO Functions
    int writeSlicesPPM(char* templateName, int sliceNr, int direction);
    int writeDX(char* Name);
    int writeOnom2K(char* Name);
    int writeToDataCube(DataCube *target);
    int writeButtonCompListToDataCube(DataCube *target);
    int TestOutPut();
    friend ostream &operator<<(ostream &ostr, const CompartmentList &CL);
    int PrintCompartmentWithId(int id);
    
    //Dataaccess
    Cellbody*  GetCellBody() {return body;};
    list<Compartment*>& GetList() {return List;};

private:
    int CheckIfThereIsInTheRNeigbourHoodABiggerDistance(Compartment *comp,double radius);
   
    int kindOfNeigbourHood; //6, 18, 26
    int ChamferDistance101417(int kind);
    int (*callBackFunction) (int percentage); // function to comunicate progress
    
    static int IDCounter;
    list<Compartment*> List;
    DataCube *dataCube;
    int NrOfCompartments;
    int id;
    int level;

//////////////////////////     Shell Model             //////////////////////////////
    list<Compartment*> *routeList;
    int maxDist; 
    list<Compartment*> buttonCompList_shellModel; //list of dendrite terminations
    list<Compartment*> linkCompList_shellModel; //list of bifuractions

/////////////////////////////////////////////////////////////////////////////////////

    list<Compartment*> buttonCompList; //list of dendrite terminations
    list<Compartment*> linkCompList; //list of bifuractions

    Compartment* bodyComp;
    Cellbody *body;

};




#endif


