/*
 *  cellloader.h
 *  full3dproj
 *
 *  Created by Philip Julian  Broser on Thu Jan 29 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include "../reconstructionlib/nervecell.h"

#include <iostream>
using namespace std;
#define ERROR_READIND_FILE -1

class CellLoader 
{
public:
    CellLoader();
    int LoadHocFile(char *name);
    int WriteHocFile(char *name);
    Cellbody *AccessCellbody() {return cellBody;};
    
    double MaximumDistanceFromCellbody();
    int PlotIntoDataCube(DataCube *target, Vector<double> aligment);
    int PlotIntoDataCube(DataCube *target);
    
    int detectSynapses(DataCube *source1, double threshold1, list< Vector<double> > &synapses);

    
    Vector<double> GetCellbodyPosition();
    
private:
    
    bool message;
    Structure *SearchStructureByName(char *name);
    Link *SearchLinkByName(char *name);
    int SetActivStructure(char *name);
    int SetActiveLink(char *name);
    int ConnectStructures(char *name1, double param1, char *name2, double param2);
    int ClearStructure(char *name);
    int CreateStructure(char *name, int kind);
    Link *CreateLink(char *Name, Structure *proximalStructure);
    int AddPoint3D(double x, double y, double z, double radius);
    
    int drawBox(DataCube *target,int posX, int posY, int posZ, double radius, float val);
    int lookForAxonInNeigbourhood(DataCube *source, double threshold, int posX, int posY, int posZ, double radius);
    
    
    list<Link*> linkList;
    list<Structure*> structureList;
    Link* activeLink;
    Structure *activeStructure;
    Cellbody *cellBody;
};

