/*
 *  viewcube.h
 *  neura
 *
 *  Created by Philip Julian  Broser on Fri Dec 12 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef VIEWCUBE
#define VIEWCUBE
#include "../datalib/datacube.h"
enum   TypeOfCreation {linear, step, maxintensity5, maxintensity10, mean}; 

typedef unsigned char ViewCubeDataType;
#define  maxValueOfViewCubeDataType 255
#define  minValueOfViewCubeDataType   0

class ViewCube 
{
public:
    ViewCube(int x, int y, int z);
    ViewCube(DataCube *source,TypeOfCreation type);
    ViewCube(DataCube *source,TypeOfCreation type, int contrast, int brigthness);//contrast, brigthness: 0..100
    ~ViewCube();
    
    
    ViewCubeDataType* GetDataPointer();
    int GetSizeX() {return sizeX;}
    int GetSizeY() {return sizeY;}
    int GetSizeZ() {return sizeZ;}
    
    ViewCubeDataType *GetSlice(int nr);
    

private:
    ViewCubeDataType* data;
    int sizeX, sizeY, sizeZ;
    int defaultC(int x, int y, int z); // allocates memory and sets size
    int createViewData(DataCube *source,TypeOfCreation type, int contrast, int brightness); //copy and converts data
    
    int linear(DataCube *source, int contrast, int brigthness);
    int maxIntensityProjection(int nrOfSlices, DataCube *source, int contrast, int brigthness);
};

#endif
