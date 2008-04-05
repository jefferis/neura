/*
 *  AxonPathTracking.cpp
 *  AxonDataProcessing
 *
 *  Created by Philip J. Broser on Tue Oct 12 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "AxonPathTracking.h"

#include "axonprocessingcore.h"
using namespace std;


int secureGetData(ucharDataType &data, int x, int y)
{
    if (x<0 || y<0 || x>=data.sizeX || y>=data.sizeY) return 0;
    return GetBinaryDataAtPoint(data,x,y);
}

void secureSetData(ucharDataType &data, int x, int y, unsigned char d)
{
    if (x<0 || y<0 || x>=data.sizeX || y>=data.sizeY) return;
    SetBinaryDataAtPoint(data,x,y,d);
}

short howManyNeigbours(ucharDataType &data, int x, int y)
{
    int dx, dy;
    short counter=0;
    for (dy=-1;dy<=1;dy++)
	for (dx=-1;dx<=1;dx++)
	    if (secureGetData(data, (x+dx), (y+dy)))
		counter++;	
    
    return counter;
}



float nextStep(ucharDataType &skelData, int x, int y,int & dx, int & dy)
{
    short counter=0;
    for (dy=-1;dy<=1;dy++)
	for (dx=-1;dx<=1;dx++)
	{
	    //cout << "dx=" << dx <<  " dy=" << dy << " Secure Get Data=" <<  secureGetData(skelData, x+dx, y+dy) << endl;
	    if (secureGetData(skelData, x+dx, y+dy)) 
	    {
		//cout << "Here is something !" << endl;
		return sqrt((float) dx*dx+dy*dy);
	    };
	};
    return 0.0;	
};


#include <list>

class Compartment
{
public:
    int x,y;
    float length;
};

#define maxRecursiceDepth 20
#define maxIterationNr 200


float trackAxon(int x, int y, ucharDataType &skelData, list<Compartment> &myAxonCompList, float minBranchLength, int recursciveDepth)
{
    Compartment myComp;
    myComp.x=x;
    myComp.y=y;
    myAxonCompList.push_back(myComp);
    secureSetData(skelData, x, y, 0);
    
    float length=0;
    if (recursciveDepth>maxRecursiceDepth) return 0.0;
    recursciveDepth++;
    int iterationNr=0;    
    
	//Walk along the axon
    bool reachedTermination=false;
    while (!reachedTermination && iterationNr<maxIterationNr)
    {
	//cout << "*" ;// at x:" << x << " and y:" << y << " "; 
	int dx, dy;
	iterationNr++;
	if (howManyNeigbours(skelData, x, y)>1)//This is a bifurcation have to walk throuh all paths
	{
		//cout << "Reached bifurcation" << endl;
	    for (dy=-1;dy<=1;dy++)
		for (dx=-1;dx<=1;dx++)
		    if (secureGetData(skelData, x+dx, y+dy)) 
		    {
			list<Compartment> branchAxonCompList;
			float branchLength=trackAxon(x+dx, y+dy, skelData, branchAxonCompList, minBranchLength, recursciveDepth);
			if (branchLength>=minBranchLength)
			{
			    length=length+branchLength;
			    myAxonCompList.splice(myAxonCompList.end(), branchAxonCompList);
			}
		    }
	}
	    
	    
	    float deltaLength=nextStep(skelData, x, y, dx, dy);
	//cout << "Delta Length:" << deltaLength << endl; 
	    if (deltaLength>0.0)
	    {
		x=x+dx;
		y=y+dy;
		length=length+deltaLength;
		Compartment comp;
		comp.x=x;
		comp.y=y;
		myAxonCompList.push_back(comp);
		secureSetData(skelData, x, y, 0);
	    }
	    else
	    {
		reachedTermination=true;
		//cout << "Reached termination Length of this axon:" << length << endl;
	    }
	}
	return length;
	
}





int plotAxonToDataSet(list<Compartment> &axonCompList,ucharDataType processedData)
{
    list<Compartment>::iterator pos;
    for (pos=axonCompList.begin();pos!=axonCompList.end();++pos)
    {
	int x=(*pos).x;
	int y=(*pos).y;
	secureSetData(processedData, x, y, 1);
    };
    return 0;
}



int senseSkeleton(ucharDataType skelData, ucharDataType processedData, int minimalbranchlengthpixel)
{
    float minLength=minimalbranchlengthpixel;
    cout << "Remove all branches shorter then:" << minLength << " pixel" << endl; 
    
    
    //Search for Axon Start Points:
    for (int y=0;y<skelData.sizeY;y++)
 	for (int x=0;x<skelData.sizeX;x++)
 	{
	    if (!secureGetData(skelData, x, y)) continue;
 //No Axon here nothing to do
	    if (howManyNeigbours(skelData, x, y)<4) 
// is it the beginning of the axon?
	    {
		cout << "Start to track axon at position " << x << " " << y << endl;
		list<Compartment> axonCompList;
		float axonLength=trackAxon(x,y, skelData,axonCompList, minLength, 0);
		if (axonLength>=minLength) 
		{
			    //cout << "this axon is long enough" << endl;		
		    plotAxonToDataSet(axonCompList, processedData);
		};
	    };
	    
 	};  
    return 0;
};   



    