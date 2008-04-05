/****************************************************************************/
/*                                                                          */
/* File:      multicompatmentlist.h 	                                    */
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


#include <stdio.h>

#include <iostream.h>
#include <list.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"
#include "../errorcode.h"
#include "compartmentlist.h"
#include "compartment.h"


#ifndef MULTICOMPARTMENTLIST_H
#define MULTICOMPARTMENTLIST_H



using namespace std;


/*!
* The Class MultiCompartmentList is to handle the several levels of reconstruction.
 * The main part is the List of CompartmentListPointers. Here the data is stored.
 * There are two different ways to use this class, 
 * 1. you can either create a octree hierachy of compartments with
 * father, son and neigbour relation or
 * 2. simpy create a graph from the data cube, that means each voxel represents a
 * compartment in the first and only compartment list
 *
 * This class splits into three types of methods:
 * 1. full functionality
 * 2. just hand the command over to the finest list
 * 3. private functional methods
 */
class MultiCompartmentList
{

public:
    MultiCompartmentList(DataCube *dC);
    MultiCompartmentList(DataCube *dC, int (*callBack) (int percentage));
    ~MultiCompartmentList();

    //1. functions to create the hieracy
    int createNextLevel(); // splits every compartment to 8 (2d to 4) compartments
    int createNextLevelSmart();// splits every compartment to 8 or 4 trys to make symetric compartments
    int ReduceFinestList(double epsilon); // delets compartments accoding to a specified algorithm
    int DeleteNonConnectedFromFinestList(); 

    //use every voxel as a compartment (2.)
    int createFinestCompartmentListFromScratch();
    
    
    //You can use this function to filter each comartment of the finest list
    int filter(FilterOptions &fo);


    //Graph extraction
    //all this opperations are performed on the finest compartmentlist
    //1.
    int SetBody(Vector<int> position);
    
    //2.
    int SetBodyDirectFromFinestList(Vector<int> position);
    
    int CalcualteAndSetDistancesFromBodyCompartment();  // set distance, cellbody to 0, neigbours to 1, neigbours of neigbours to n+1,...
    int CalcualteAndSetDistancesFromBodyCompartment27();  // set distance, cellbody to 0, neigbours to 1, neigbours of neigbours to n+1,...
    int CalcualteAndSetDistancesFromBodyCompartmentUsingChamferFunction(); // set distance, cellbody to 0, and neigbours due to euklidean approximation 10,14,17
    int CalcualteAndSetDistancesFromBodyCompartment_ShellModel(); //set distance, cellbody to 0 and neighbours due to the shell distance 
    
	int SearchLokalMaximaAndCreateButtonCompList_ShellModel(); // detect dendrite terminations
	int SearchLokalMaximaOfDistance6AndCreateButtonCompList(); 
    int SearchLokalMaximaOfDistance27AndCreateButtonCompList();
    int SearchLokalMaximaOfDistance27SharpAndCreateButtonCompList();
	
	int DetectCellbody(list<Vector<double> > *cellbodyList, double percentage);
    
    int CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm);
    int CheckIfButtonsInButtonCompListAreMeaningfullAndDeleteOther_ShellModel(double minDistanceFromBodyCompartment, double minLengthOfDendriteInqm);
    int MarkBoundaryCompartments();
    int ExtractSkeleton();
    int ExtractSkeleton2();
    
    
    int SetNeigbour27();
    
    //creates a tree representation of the neuron
    int WalkFromButtonsToBodyAndCreateNeuronStructure();

    
   

    
    
   //User Interface for dendrite terminations
    int createMarkList(list<Mark*> &markList);
    int AddMark(Mark &newMark);
    int DeleteMark(int compartmentID);


 //IO functions
    friend ostream &operator<<(ostream &ostr, const MultiCompartmentList &MCL);
    int writeSlicesPPM(int level,char* templateName, int sliceNr, int direction);
    int writeOnom2K(int level,char* Name);
    int writeDX(int level,char* Name);
    int writeToDataCube(DataCube *target);
    int writeButtonCompListToDataCube(DataCube *target);
    int writeTestOutPut();

    //Dataaccess
    Cellbody* GetCellBody();
    
   


        

private:
    // function to communicate progress
    int (*callBackFunction) (int percentage); 
   
    //methods to search for body compartment
    //1.
    Compartment* FindFinestCompartmentSurrounding(Vector<int> position); 
    Compartment* FindFinestCompartmentSurrounding(Compartment *comp,Vector<int> position);
    
    //2.
    Compartment* FindFinestCompartmentSurroundingInFinestList(Vector<int> position);
    
    //methods to create next level of the octree representation
    int splitCompartment2D(Compartment *comp,Compartment ***field,int& nr); // splits 2D comp into 4
    int splitCompartment3D(Compartment *comp,Compartment ***field,int& nr); // splits compartment into 8 ones Octree approach
    int splitCompartment(Compartment  *comp,Compartment ***field,int& nr);
    int singleChildCompartment(Compartment  *comp,Compartment **singleChild);

    //Auxillary Function for Scratch List
    Compartment* DirektAcessCompartment(Compartment** field, int posX, int posY, int posZ, short dX, short dY, short dZ);
    Compartment* DirectNeigbourAccess(Compartment** field, int posX, int posY, int posZ, int neigbour);
    
    DataCube *dataCube; //underlying data
    list<CompartmentList*> compartmentList; //compartmentlist hierachy
    int presentlevel; 
    int maxlevel;

    Compartment* bodyComp; // acess to nervecell structure (access to cellbody)


};

#endif


