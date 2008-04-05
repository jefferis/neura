/*
 *  cellloader.cpp
 *  full3dproj
 *
 *  Created by Philip Julian  Broser on Thu Jan 29 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "cellloader.h"
extern "C"
{
#include "hocparser.h"
}
#define OPENBRACKET 1
#define CLOSEBRACKET 2
#define SEPERATOR 3 
#define DEFFENITION 4
#define NUMBER 5
#define PT3DADD 6
#define ACCESS 7
#define CREATE 8
#define PT3DCLEAR 9
#define VARIABLE 10
#define NEWLINE 11
#define CONNECT 12
#define DONE 0

#include "../datalib/vector.h"

CellLoader::
CellLoader()
{
    message=false; // how much information ouput? 
    activeLink=NULL;
    activeStructure=NULL;
};

Structure*
CellLoader::
SearchStructureByName(char *name)
{
    
    list<Structure*>::iterator pos;
    for (pos=structureList.begin();pos!=structureList.end();++pos)
        if ( (*pos) ->IsThisMyName(name) )
	    return (*pos);
    return NULL;
}

Link*
CellLoader::
SearchLinkByName(char *name)
{
    list<Link*>::iterator pos;
    for (pos=linkList.begin();pos!=linkList.end();++pos)
        if ( (*pos)->IsThisMyName(name) )
	    return (*pos);
    return NULL;

}

int 
CellLoader::
SetActivStructure(char *name)
{
    activeStructure=SearchStructureByName(name);
    if (activeStructure==NULL) return 1;
    return 0;
}

int 
CellLoader::
SetActiveLink(char *name)
{
    activeLink=SearchLinkByName(name);
    if (activeLink==NULL) return 1;
    return 0;
}

int 
CellLoader::
ConnectStructures(char *name1, double param1, char *name2, double param2)
{
    Structure *structure1=SearchStructureByName(name1);
    if (structure1==NULL) {printf("Can not access %s\n",name1);return ERROR_READIND_FILE; };
    Structure *structure2=SearchStructureByName(name2);
    if (structure2==NULL) {printf("Can not access %s\n",name1);return ERROR_READIND_FILE; };
    Structure *distal, *proximal;
    
    if (param1>param2) {proximal=structure1;distal=structure2;} else {proximal=structure2;distal=structure1;};
    
    Link* link=SearchLinkByName(proximal->GetName());
    if (link==NULL) 
    {
	if (message) printf("There is no link for structure %s will create one\n",proximal->GetName());
	link=CreateLink(proximal->GetName(), proximal);
    }
    link->AddDistalStructure(distal);
        proximal->SetDistalLink(link);
        distal->SetProximalLink(link);
    
    if (message) printf ("Connected proximal structure %s with distal structure %s via Link %s\n",proximal->GetName(),distal->GetName(),link->GetName());
    
    return 0;
}

int 
CellLoader::
ClearStructure(char *name)
{
    Structure* structure=SearchStructureByName(name);
    // not implemented yet
    return 0;
}


int 
CellLoader::
CreateStructure(char *name, int kind)
{
    Structure *structure;
    switch (kind)
    {
	case DENDRITE : structure=new Dendrite(); break;
	case BUTTON   : structure=new Button();break;
	case CELLBODY : cellBody=new Cellbody();structure=cellBody;break;
    }
    structure->SetName(name);
    structure->SetBase(1);
    structureList.push_back(structure);
    return 0;
}

Link* 
CellLoader::
CreateLink(char *Name, Structure *proximalStructure)
{
    Link *lLink=new Link();
    lLink->SetBase(1);
    lLink->SetName(Name);
    lLink->SetProximalStructure(proximalStructure);
    linkList.push_back(lLink);
    return lLink;
}

int 
CellLoader::
AddPoint3D(double x, double y, double z, double radius)
{
    if (activeStructure==NULL) return 1;
    Vector<double> pt3d(4);
    pt3d[1]=x;
    pt3d[2]=y;
    pt3d[3]=z;
    pt3d[4]=radius;
    activeStructure->addPoint3D(pt3d);
    return 0;
}


//"/Volumes/Data/cfd/Full3dprojection/example.hoc"

int 
CellLoader::
LoadHocFile(char *name)
{
    cout << "Hoc File Parser started read file:" << name << endl;
    if (openHocFile(name)!=OK) {printf("Error reading file.\n");return 1;};
    int tag=1;
    while (tag)
    {
	tag=readTag();
	if (tag==ERRORREADINGFILE)
	{
	    printf("ERRORREADINGFILE\n");
	    return ERROR_READIND_FILE;
	}
	
	switch (tag)
	{
	    
	    case PT3DADD : 
	    {
	
		if (AddPoint3D(pt3dVector[0],pt3dVector[1],pt3dVector[2],pt3dVector[3])) return ERROR_READIND_FILE;
		if (message) printf("Add (%f,%f,%f,%f) to %s\n",pt3dVector[0],pt3dVector[1],pt3dVector[2],pt3dVector[3],activeStructure->GetName());
		break;
	    }
	    case ACCESS  : 
	    {
		if (SetActivStructure(name1))
		{
		    printf("can not access(%s) , ERROR\n",name1); 
		    return ERROR_READIND_FILE;
		}
		break;
	    };
	    case CREATE  : 
	    {
		if (strcmp(name1,"Cellbody")==0) 
		{
		    if (message) printf("create(%s) as CELLBODY\n",name1);
		    CreateStructure(name1, CELLBODY);
		    break;
		}
		if (message) printf("create(%s) as DENDRITE\n",name1);
		CreateStructure(name1, DENDRITE);
		break;
	    };
	    case PT3DCLEAR : if (message) printf(" return pt3dclear() \n");break;
	    case DEFFENITION :  
	    {
		if ( (strcmp(name1,"nseg")==0) && (activeStructure!=NULL))
		{
		    activeStructure->SetNrOfSeg((int) value1);
		    if (message) printf("Set nrofseg of %s to %d\n",activeStructure->GetName(), (int) value1);break;
		    break;
		}
		printf("return defention:%s= %f\n",name1,value1);break;
	    
	    }
		
	    case CONNECT : 
	    {
		Structure *structure1=SearchStructureByName(name1);
		if (structure1==NULL) {printf("Can not access %s\n",name1);return ERROR_READIND_FILE; };
		Structure *structure2=SearchStructureByName(name2);
		if (structure2==NULL) {printf("Can not access %s\n",name1);return ERROR_READIND_FILE; };
		if (message) printf(" return connect: %s(%f) with %s(%f)\n",name1,value1,name2,value2) ;
		ConnectStructures(name1, value1, name2, value2);
		break;
	    };
	    case NEWLINE: ; break;   
	    case DONE: ;break;
	    default: if (message) printf("No tag detected:%d\n",tag);break;
	}    
	
    }

    printf("Close file\n");
    closeFile();
    return 0;
}


int 
CellLoader::
WriteHocFile(char *name)
{
    //not implemented yet
    return 0;
}

Vector<double>
CellLoader::
GetCellbodyPosition()
{
    Vector<double> position(3);
    position[1]=(*((cellBody->AccsessPoints3D()).begin()))[1];
    position[2]=(*((cellBody->AccsessPoints3D()).begin()))[2];
    position[3]=(*((cellBody->AccsessPoints3D()).begin()))[3];
    return  position;

}




double
CellLoader::
MaximumDistanceFromCellbody()
{
    Vector<double> bodyPosition=GetCellbodyPosition();
    double dist=0;
    list<Structure*>::iterator pos;
    for (pos=structureList.begin();pos!=structureList.end();++pos)
    {
	list< Vector<double> >::iterator pos2;
	 for (pos2= ((*pos)->AccsessPoints3D()).begin();pos2!=((*pos)->AccsessPoints3D()).end();++pos2)
	 {
	     Vector<double> distV=(*pos2)-bodyPosition;
	     if (Norm(distV)>dist) dist=Norm(distV);
	 }
	
    };
	
    return dist;
}

int 
CellLoader::
drawBox(DataCube *target,int posX, int posY, int posZ, double radius, float lval)
{
    double factor=5.0;
    int sx, sy, sz;
    sx=(int )((double)factor*radius);
    sy=(int )((double)factor*radius);
    sz=(int )((double)factor*radius);
    
    for (int z=-sz;z<=sz;z++)
	 for (int y=-sz;y<=sy;y++)
	      for (int x=-sx;x<=sx;x++)
	      {
		  if (sqrt((double)x*x+y*y+z*z)>factor*radius) continue;
		  float val;
		  val=exp(-0.1*(x*x+y*y+z*z))/exp(0.0)*lval;
		  target->SetData(posX+x,posY+y,posZ+z, target->GetData(posX+x,posY+y,posZ+z)+val);
	      
	      };
return 0;
    
}

int 
CellLoader::
lookForAxonInNeigbourhood(DataCube *source, double threshold, int posX, int posY, int posZ, double radius)
{
    double factor=1.0;
    int sx, sy, sz;
    sx=(int )((double)factor*radius);
    sy=(int )((double)factor*radius);
    sz=(int )((double)factor*radius);
    
    for (int z=-sz;z<=sz;z++)
	for (int y=-sz;y<=sy;y++)
	    for (int x=-sx;x<=sx;x++)
	    {
		if (source->GetData(posX+x,posY+y,posZ+z)>threshold) return 1;
	    };
    
     return 0;
}


int 
CellLoader::
PlotIntoDataCube(DataCube *target, Vector<double> aligment) // aligment is the position where the cellbody shoud be maped to
{   
    Vector<double> moveVector=GetCellbodyPosition()-aligment;
    Vector<double> plotPosition(3);
    Vector<double> scaleVector=target->GetSpaceing();
   
    
    // Big Ball Arround Cellbody for Debuging
    /*
    int sx, sy, sz;
    sx=5;
    sy=5;
    sz=5;
    int posX=((GetCellbodyPosition()-moveVector)[1]/scaleVector[1]);
    int posY=((GetCellbodyPosition()-moveVector)[2]/scaleVector[1]);
    int posZ=((GetCellbodyPosition()-moveVector)[3]/scaleVector[1]);
    
    
    
    for (int z=-sz;z<=sz;z++)
	for (int y=-sz;y<=sy;y++)
	    for (int x=-sx;x<=sx;x++)
	    {
		target->SetData(posX+x,posY+y,posZ+z, target->GetData(posX+x,posY+y,posZ+z)+1.0);
	    };

     */
    //
    
    
    
    list<Structure*>::iterator pos;
    for (pos=structureList.begin();pos!=structureList.end();++pos)
    {
	list< Vector<double> >::iterator pos2;
	for (pos2= ((*pos)->AccsessPoints3D()).begin();pos2!=((*pos)->AccsessPoints3D()).end();++pos2)
	{
	    Vector<double> pos3d(3);
	    pos3d[1]=(*pos2)[1];
	    pos3d[2]=(*pos2)[2];
	    pos3d[3]=(*pos2)[3];
	    plotPosition=pos3d-moveVector;
	    int posX= (int)  ( (double) plotPosition[1]/scaleVector[1]);
	    int posY= (int)  ( (double) plotPosition[2]/scaleVector[2]);
	    int posZ= (int)  ( (double) plotPosition[3]/scaleVector[3]);
	    drawBox(target,posX,posY,posZ,(*pos2)[4],1.0);
	    
	    
	}
	
    };
    

    
    

    return 0;
}





int 
CellLoader::
PlotIntoDataCube(DataCube *target) // no aligment 
{   
    Vector<double> moveVector(3);
    moveVector[1]=0.0;
    moveVector[2]=0.0;
    moveVector[3]=0.0;
    Vector<double> plotPosition(3);
    Vector<double> scaleVector=target->GetSpaceing();
    
    
    // Big Ball Arround Cellbody for Debuging
    /*
     int sx, sy, sz;
     sx=5;
     sy=5;
     sz=5;
     int posX=((GetCellbodyPosition()-moveVector)[1]/scaleVector[1]);
     int posY=((GetCellbodyPosition()-moveVector)[2]/scaleVector[1]);
     int posZ=((GetCellbodyPosition()-moveVector)[3]/scaleVector[1]);
     
     
     
     for (int z=-sz;z<=sz;z++)
     for (int y=-sz;y<=sy;y++)
	    for (int x=-sx;x<=sx;x++)
	    {
		target->SetData(posX+x,posY+y,posZ+z, target->GetData(posX+x,posY+y,posZ+z)+1.0);
	    };
     
     */
    //
    
    
    
    list<Structure*>::iterator pos;
    for (pos=structureList.begin();pos!=structureList.end();++pos)
    {
	list< Vector<double> >::iterator pos2;
	for (pos2= ((*pos)->AccsessPoints3D()).begin();pos2!=((*pos)->AccsessPoints3D()).end();++pos2)
	{
	    Vector<double> pos3d(3);
	    pos3d[1]=(*pos2)[1];
	    pos3d[2]=(*pos2)[2];
	    pos3d[3]=(*pos2)[3];
	    plotPosition=pos3d-moveVector;
	    int posX= (int)  ( (double) plotPosition[1]/scaleVector[1]);
	    int posY= (int)  ( (double) plotPosition[2]/scaleVector[2]);
	    int posZ= (int)  ( (double) plotPosition[3]/scaleVector[3]);
	    double radius=(*pos2)[4];
	    radius=1.0;//debug
	    drawBox(target,posX,posY,posZ,radius,1.0);
	    
	    
	}
	
    };
    
    
    
    
    
    return 0;
}


int 
CellLoader::
detectSynapses(DataCube *source, double threshold1, list< Vector<double> > &synapses)
{
    Vector<double> moveVector(3);
    moveVector[1]=0.0;
    moveVector[2]=0.0;
    moveVector[3]=0.0;
    
    
    Vector<double> scaleVector=source->GetSpaceing();
    Vector<double> pos3d(4);
    Vector<double> plotPosition(4);
    
    list<Structure*>::iterator pos;
    for (pos=structureList.begin();pos!=structureList.end();++pos)
    {
	list< Vector<double> >::iterator pos2;
	for (pos2= ((*pos)->AccsessPoints3D()).begin();pos2!=((*pos)->AccsessPoints3D()).end();++pos2)
	{
	    
	    pos3d[1]=(*pos2)[1];
	    pos3d[2]=(*pos2)[2];
	    pos3d[3]=(*pos2)[3];
	    plotPosition=pos3d-moveVector;
	    plotPosition[1]=plotPosition[1]/scaleVector[1];
	    plotPosition[2]=plotPosition[2]/scaleVector[2];
	    plotPosition[3]=plotPosition[3]/scaleVector[3];
	    plotPosition[4]=(*pos2)[4];
	    
	    int posX= (int)  plotPosition[1];
	    int posY= (int)  plotPosition[2];
	    int posZ= (int)  plotPosition[3];
	    double radius=plotPosition[4];
	    radius=1.0;//debug
	    if (lookForAxonInNeigbourhood(source, threshold1, posX,posY,posZ,radius)) 
	    {
		cout << "Plot Position:" << endl << plotPosition << " physical position:" << endl << *pos2 << endl;
		synapses.push_back(*pos2);
				
	    }
	   
	    	
	}
	
    };
    
    
    


    return 0;
}





