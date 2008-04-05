/****************************************************************************/
/*                                                                          */
/* File:      nervecell.h 	                                            */
/*                                                                          */
/* Purpose:   class definition of neuron cell                               */
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
#include <list.h>
#include <string>
#include <strstream> 
#include "../errorcode.h"

#include "compartment.h"


#ifndef NERVECELL
#define NERVECELL

#define CELLBODY 1
#define DENDRITE 2
#define BUTTON 3

class Link;

class Structure
{
public:
    Structure();
    ~Structure();
    int SetName(char *lName);
    int SetBase(int b) {base=b; return 0;}
    char *GetName() {return name;};
    list< Vector<double> > & AccsessPoints3D() {return pt3dPoints;};
    
    int SetProximalLink(Link *link);
    int SetDistalLink(Link *link);
    int SetCompartment(Compartment *lcomp);

    Link *GetProximalLink() {return proximal;};
    Link *GetDistalLink() {return distal;}

    int GetKind() {return kind;};
    int GetId() {return id;};
    Compartment *GetComp() {return comp;};
    int SetGeometricComps(list<Compartment*> geo) {geometricComps=geo; return 0;};

    friend ostream &operator<<(ostream &ostr, const Structure &structure);
    friend int writeToDataCube(DataCube *targetDC, const Structure &structure);
    friend int createDataForDX(strstream &radiusData, strstream &positionData, strstream &connectionData, int &nrofposition, int &nroflastlink, const Structure &structure );

    int measureRadii(double threshold);

    //to search with in the list of structures
    int IsThisMyName(char *supposedName); // returns 1 if its the name of the structure
    
    //pt3d Modell
    int addPoint3D(Vector<double> &pt3d);
    
    int SetNrOfSeg(int nr) {nrOfSeg=nr;return 0;};
    
protected:
    char name[40];
    static int IDCounter;
    int id;
    int kind;

    int nrOfSeg;
    
    Link *proximal;
    Link *distal;

    
    //Two nervecell concepts, either based on compartments or on Vector data pt3d
    int base; // 0 is comparment based , 1 is pt3d based
    //
    Compartment *comp;

    list<Compartment*> geometricComps;
    list<double> radii;

    //pt3d Modell
    list< Vector<double> > pt3dPoints; //Start from proximal vectors and continue to distal

    
    
};


class Link
{
public:
    Link();
    int SetProximalStructure(Structure *prox);
    int AddDistalStructure(Structure *dist);
    int SetCompartment(Compartment *lcomp) {comp=lcomp;return 0;};
    Structure *GetProximalStructure(){return proximal;};
    list<Structure*>& GetDistalStructure(){return distal;}
    Compartment* GetComp() {return comp;};

    friend ostream &operator<<(ostream &ostr, const Link &link);

    friend int writeToDataCube(DataCube *targetDC, const Link &link);
    friend int createDataForDX(strstream &radiusData, strstream &positionData, strstream &connectionData, int &nrofposition, const Link &link );

    
    int measureRadii(double threshold);
    
    int IsThisMyName(char *supposedName); // returns 1 if its the name of the link
    int SetName(char *lName);
    int SetBase(int b) {base=b; return 0;}
    char *GetName() {return name;};

private:
    char name[40];
    static int IDCounter;
    int id;
    
    int base; // 0 is comparment based , 1 is pt3d based
    Compartment *comp;
    
    Structure *proximal;
    list <Structure*> distal;

};

class Cellbody : public Structure
{
public:
    Cellbody();

private:

};

class Dendrite : public Structure
{
public:
    Dendrite();

private:


};

class Button : public Structure
{
public:
    Button();


};





#endif
