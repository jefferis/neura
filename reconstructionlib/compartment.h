/****************************************************************************/
/*                                                                          */
/* File:      compartment.h 	                                            */
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
#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../datalib/datacube.h"

#include <iostream.h>
#include <list>
#include <list.h>
#include <string>
#include "../errorcode.h"

#ifndef COMPARTMENT_H
#define COMPARTMENT_H

using namespace std;

#define X_DIR 1
#define Y_DIR 2
#define Z_DIR 3


#define ORIGIN -1

//son Top/bottom left/right front/rear

#define SONBASENR 0
//Top Left/Right Front/Rear
#define S_TopLeftFront   0
#define S_TopRightFront  1
#define S_TopLeftRear    2
#define S_TopRightRear   3
//Bottom Left/Right Front/Rear
#define S_BottomLeftFront  4
#define S_BottomRightFront 5
#define S_BottomLeftRear   6
#define S_BottomRightRear  7

#define S_SingleChild 8



#define NROFCHILDKINDS 9
//  neigbours
//top Front/Back Right/Left
#define N_TopFront      0+NROFCHILDKINDS
#define N_TopBack       1+NROFCHILDKINDS
#define N_TopFrontRight 2+NROFCHILDKINDS
#define N_TopRight      3+NROFCHILDKINDS
#define N_TopLeft       4+NROFCHILDKINDS
#define N_TopFrontLeft  5+NROFCHILDKINDS
#define N_TopBackLeft   6+NROFCHILDKINDS
#define N_TopBackRight  7+NROFCHILDKINDS
#define N_Top           8+NROFCHILDKINDS
//samelevel
#define N_Right         9+NROFCHILDKINDS
#define N_FrontRight   10+NROFCHILDKINDS
#define N_Back         11+NROFCHILDKINDS
#define N_Left         12+NROFCHILDKINDS
#define N_FrontLeft    13+NROFCHILDKINDS
#define N_BackLeft     14+NROFCHILDKINDS
#define N_BackRight    15+NROFCHILDKINDS
#define N_Front        16+NROFCHILDKINDS
//bottom
#define N_BottomRight  17+NROFCHILDKINDS
#define N_BottomFrontRight 18+NROFCHILDKINDS
#define N_BottomBack  19+NROFCHILDKINDS
#define N_BottomLeft  20+NROFCHILDKINDS
#define N_BottomFrontLeft 21+NROFCHILDKINDS
#define N_BottomBackLeft  22+NROFCHILDKINDS
#define N_BottomBackRight 23+NROFCHILDKINDS
#define N_BottomFront     24+NROFCHILDKINDS
#define N_Bottom    25+NROFCHILDKINDS

#define FATHER 26+NROFCHILDKINDS

#define Left   -1
#define Right   1
#define Back   -1
#define Front   1
#define Bottom -1
#define Top     1
#define Middle 0

//Flags
#define STRUCTUREONMYSURFACE 1
#define SINGLE 2
#define LOCKED 4
#define BOUNDARYCOMP 8
#define BODY   16
#define ENDPOINT 32
#define SKELETONCOMP 64
#define BIFURCATION  128
#define NOTCONNECTEDTOBODY 256
#define BODYELEMENT 512

//Values
#define MAXARRIVALTIME 30000

class Link;
class Structure;

class Compartment;

/*! This class is necessary to handle connections between compartments like neigbours etc.
*
*/
class Connection
{
public:
    Connection() {};
    Connection(short lkind,Compartment *lfrom,Compartment *lto) : kind(lkind), from(lfrom), to(lto) {;};
    short kind;
    Compartment *from;
    Compartment *to;
    friend int operator==(const Connection &A, const Connection &B);
    friend int isKindEqual(const Connection &A,const short kind);
    friend ostream &operator<<(ostream &ostr, const Connection &CON);


};


/*! A Compartment represents a fixed entity of the data set. Usually that means one compartemnt represents one voxel of 
* the neuron structure.
*/
class Compartment : public Volume
{
public:
    Compartment(DataCube *dC,int llevel, const Vector<int> &lstart,const Vector<int> &lsize,Compartment *lfather, int lpositionInFatherComp);
    ~Compartment();

    //Access to underlying datacube
    NeuraDataType &operator ()(int x, int y);         // 2D acess
    NeuraDataType &operator ()(int x, int y, int z);  // 3D acess
    NeuraDataType &operator [](Vector<int> X);

    //IO Functions
    int writePPM2D (char *filename);
    int writeSlicePPM (char *filename,int sliceNr, int direction);
    friend ostream &operator<<(ostream &ostr, const Compartment &DC);
    int writeToDataCube(DataCube *target);

    //Data access
    DataCube*    GetDataCube() {return dataCube;};
    Compartment* GetConnection(short kind);
    Compartment* GetNeigbourThrouhPositionIdentifier(int neigbourOfFather, int compInNeigbour);
    Compartment* Neigbour(short kind) {return GetConnection(kind);};
    Compartment* NeigbourOfNeigbour(short Neigbour1,short Neigbour2);
    Compartment* NeigbourOfNeigbourOfNeigbour(short Neigbour1,short Neigbour2,short Neigbour3);

///////////////////////////////////////////////////////////////////////////////
///   shell model 
//////////////////////////////////////////////////////////////////////////////
    
    int SetShellModelDistanceFromBody(int dist) {shellModelDistanceFromBody=dist;return dist;};
    int GetShellModelDistanceFromBody() {return shellModelDistanceFromBody;};
    int SetShellIndex(list<int> index);
    int AddShellIndex(int index) {shellIndex.push_front(index);};
    list<int> GetShellIndex() {return shellIndex;};

//////////////////////////////////////////////////////////////////////////////

    int SetDistanceFromBody(int dist) {distanceFromBody=dist;return dist;};
    int GetDistanceFromBody() {return distanceFromBody;};
    int GetNrOfNeigbours() {return nrOfNeigbours;};
    int GetID(){return id;};
    int GetLevel() {return level;};
    int GetFlags() {return flags;};
    int GetNrOfPassingThrough() {return nrPasingThrouh;};
    int SetNrOfPassingThrough(int nr) {nrPasingThrouh=nr;return nrPasingThrouh;};
    int GetNrOfLinkConnections() {return nrOfLinkConnections;};
    int SetNrOfLinkConnections(int nr) {nrOfLinkConnections=nr;return nrOfLinkConnections;}

    //evaluate underlying datacube
    int IsThereStructureOnMySurface(double epsilon);
    double CalculateRadius(double threshold, int choice);
    double BigestBallAroundBeginningWithSmallBall(double threshold);
    double BigestBallAroundBeginningWithHugeBall(double threshold);
   
    //neigbourhood
    int FindAndSetMyNeigbours6();
    int FindAndSetMyNeigbours27();
    int AmIASingleOrDoIHaveNeigbours();
    int AmIABoundaryComp();
    int AmIABoundaryComp(short neigbourhoodType);
    int isTheDistanceFromASecondOrderNeigbourBigger();
    int isTheDistanceFromAThirdNeigbourBigger();

    // These function are used to navigate through the graph structure.
    // Usually they work recursive.
    int MarchToCalcNrOfPasingThrouhAndDetectBifurations(Compartment *bodyComp);
    int MarchToAndAddPassingThroughs(Compartment *bodyComp);
    Compartment *MarchtToNextAndMeasureDistance(int searchFlag, int &distance);
    Compartment *MarchToNext(int searchFlag,list<Compartment*>  &geometricComps);
    int MarchTo6(Compartment *bodyComp);
    Compartment *MarchToNext6(int searchFlag,list<Compartment*>  &geometricComps);
    int addPassingThrough(Compartment *comp);


    //handles list of neigbours, sons and father
    int addToConnectionList(Connection &con);
    int addToConnectionList(short kind, Compartment *to);
    int addToReverseConnectionList(Connection &con);

    int SetNeigbour(Compartment *neigbour, int position);
    int SetSon(Compartment *son, int nr);

    int deleteFromConnectionList(Connection &con);
    int deleteFromReverseConnectionList(Connection &con);



    //Access to controll flags
    int SETFLAG(int f) {return flags=flags | f;};
    int CLEARFLAG(int f) {return flags=(flags | f) - f;};
    int FLAG(int f) {return flags & f;};


    //These functions connect the coresspondending parts of the nervecell with the compartment
    int SetStructure(Structure *s) {structure=s;return 0;};
    int SetLink(Link *l) {link=l;return 0;};
    Structure* GetStructure() {return structure;};
    Link* GetLink() {return link;};

    //Auxillary functions
    int IsPointInCompartment(int gx,int gy,int gz);
    int IsPointInCompartment(Vector<int> position);


private:
    static int IDCounter;
    DataCube *dataCube;
    int id;
    int level;

    int distanceFromBody;

    ////////////////////////       Shell Model       //////////////////////////////
    int shellModelDistanceFromBody;
    list<int> shellIndex;
    ///////////////////////////////////////////////////////////////////////////////

    short positionInFatherComp;
    int flags; // Controll flags

    int nrOfNeigbours;
    list<Connection> ConnectionList;
    list<Connection> reverseConnectionList;


    list<Compartment*> pasingThrouh;
    int nrPasingThrouh;
    int nrOfLinkConnections;

    Link *link;
    Structure *structure;

    double localThreshold;

};


#endif













