/*
 *  viewcube.cpp
 *  neura
 *
 *  Created by Philip Julian  Broser on Fri Dec 12 2003.
 *  Copyright (c) 2003 MPIMF-Heidelberg. All rights reserved.
 *
 */

#include "viewcube.h"


ViewCubeDataType* 
ViewCube::
GetDataPointer()
{
    //cout << "ViewCube::request for data pointer" << endl << flush;
    return data;
}

ViewCubeDataType *
ViewCube::
GetSlice(int nr)
{
    if (nr<0) nr=0;
    if (nr>=sizeZ) nr=(sizeZ-1);
    return &(data[nr*sizeX*sizeY]);
}



ViewCube::
~ViewCube()
{
    delete [] data;
};

int 
ViewCube::
defaultC(int x, int y, int z)
{
    cout << "Create new view cube..." << flush;
    sizeX=x;
    sizeY=y;
    sizeZ=z;
    int fieldSize=x*y*z;
    data=new ViewCubeDataType[fieldSize];
    if (data==NULL) 
    {
	cout << "No Memory left! Quit!" << endl;
	exit(1);
    }
    
    cout << "done. Size :"<< sizeX << "*" << sizeY << "*" << sizeZ << endl << flush; 
    return 0;

}




ViewCube::
ViewCube(int x, int y, int z)
{
    defaultC(x, y,z); 
};




int 
ViewCube::
createViewData(DataCube *source,TypeOfCreation type, int contrast, int brightness)
{
    switch (type)
    {
	
	case maxintensity5:
	    maxIntensityProjection(5,source,contrast, brightness);
	    break;
	case maxintensity10: 
	    maxIntensityProjection(10,source, contrast, brightness);
	    break;
	    
	case step:
	case mean:
	default:
	    linear(source,contrast, brightness);
	    break;
    }
    return 0;
}



ViewCube::
ViewCube(DataCube *source,TypeOfCreation type)
{
    defaultC(source->GetSize()[1],source->GetSize()[2],source->GetSize()[3]);
    createViewData(source,type, 50, 50);  
}




ViewCube::
ViewCube(DataCube *source,TypeOfCreation type, int contrast, int brightness)
{
    defaultC(source->GetSize()[1],source->GetSize()[2],source->GetSize()[3]);
    createViewData(source,type, contrast, brightness); 
}

 



//Types of data creation
int 
ViewCube::
linear(DataCube *source,int contrast, int brightness)
{
    
    cout << "wd:Write data using linear look up table..." << endl << flush;
    cout << "contrast:" << contrast <<  "(0..100)" << endl;
    cout << "brightness:" << brightness <<  "(0..100)" <<endl;
    
    long nrOfElements=source->NrOfElements();
    cout << "Source cube has " << nrOfElements << " elements" << endl << flush;
    NeuraDataType *sourceDataPointer=source->GetDataPointer();
    NeuraDataType min,max;
    source->CalcualteMinAndMax(min,max);
    cout << "min value:" << min << endl;
    cout << "max value:" << max << endl;
    NeuraDataType scaleFactor= (NeuraDataType) maxValueOfViewCubeDataType/max;
    cout << "pre scale factor:" << scaleFactor << endl << flush;
    NeuraDataType factor = (contrast)/50.0;
    NeuraDataType offset = (brightness);
    if (nrOfElements>sizeX*sizeY*sizeZ) 
    {
	cout << "wrong field size quit" << endl;
	exit(1);
    }
    NeuraDataType help;
    long i;
    for(i=0;i<nrOfElements;i++)
    { 
	help =  ( (NeuraDataType) scaleFactor*sourceDataPointer[i]); //sets data between 0..255
	help = help*factor + offset;
	if (help<  0.0) {data[i]=0;continue;}
	if (help>254.0) {data[i]=255;continue;}
	data[i]= (ViewCubeDataType) help;
    };
    cout << "wd:done." << endl << flush;
    return 0;
}


int 
ViewCube::
maxIntensityProjection(int nrOfSlices, DataCube *source,int contrast, int brightness)
{
    NeuraDataType localmax, present, min, max;
    source->CalcualteMinAndMax(min,max);
    cout << "min value:" << min << endl;
    cout << "max value:" << max << endl;
    NeuraDataType scaleFactor= (NeuraDataType) maxValueOfViewCubeDataType/max;
    cout << "scale factor:" << scaleFactor << endl << flush;
    cout << "create view cube using max intensity projection using" << nrOfSlices << " slices" << endl; 
    cout << "contrast:" << contrast <<  "(0..100)" <<endl;
    cout << "brightness:" << brightness <<  "(0..100)" <<endl;
    NeuraDataType factor = (contrast+50.0)/50.0;
    NeuraDataType offset = (brightness);
    NeuraDataType help;
	for (int z=source->GetStart()[3];z<source->GetSize()[3];z++)
	{
	    for (int y=source->GetStart()[2];y< source->GetSize()[2];y++)
		for (int x=source->GetStart()[1];x< source->GetSize()[1];x++)
		{
		    localmax=source->GetData(x,y,z);
		    for(int i=-nrOfSlices;i<nrOfSlices;i++)
		    {
			present=source->GetData(x,y,z+i);
			if(localmax<present) max=present;
		    };
		    
		    help =  ( (NeuraDataType) scaleFactor*localmax); //sets data between 0..255
		    help = help*factor + offset; //linear lookup table
		    if (help<  0.0) {data[x+y*sizeX+z*sizeX*sizeY]=0;continue;}
		    if (help> 254.0) {data[x+y*sizeX+z*sizeX*sizeY]==255;continue;}
		    data[x+y*sizeX+z*sizeX*sizeY]=  (ViewCubeDataType) help;
		    
		};
	};
    cout << "done" << endl;
    return 0;
};









