/*
 *  tophat.c
 *  DistanceMap
 *
 *  Created by Philip J. Broser on Mon Aug 23 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdlib.h>
#include "tophat.h"
#include <math.h>



#define INTEGRATE(RANGE, FUNCTION) \
int x,y; \
for (y=-RANGE ; y<=+RANGE ; y=y+1) \
for (x=-RANGE; x<=+RANGE; x=x+1) \
{ FUNCTION; }; 

float dilateX (densityDataType* data, intVectorType x0, int range)
{
    float max,b;
    b=0;
    max=0;
    range=range/2;
    int x;
    for (x=-range; x<=+range; x=x+1)
    {
	b=ACCESSDATAATPOINT((*data), x0.x-x, x0.y);
	if (b>max) max=b;
    }
    return max;
  // printf("max: %f\n", max);
}

float dilateY (densityDataType* data, intVectorType x0, int range)
{
    float max,b;
    b=0;
    max=0;
    range=range/2;
    int y;
    for ( y=-range; y<=+range; y=y+1)
    {
	b=ACCESSDATAATPOINT((*data), x0.x, x0.y-y);
	if (b>max) max=b;
    }
    return max;
  //printf("max: %f\n", max);
}

int DilationXY(densityDataType* data, int range, densityDataType* dilation)
{ 
    int height=(*data).sizeY;
    int width=(*data).sizeX;
    int imagesize=height*width+1;
    densityDataType help;
    (help).sizeX=width;
    (help).sizeY=height;
    (help).data=(float*) malloc( sizeof(float) * imagesize);
    int n=range/2;
    int x,y;
    for ( y=n+10;y<(*data).sizeY-(n+10);y++)
    {
	for ( x=n+10;x<(*data).sizeX-(n+10);x++)
	{
	  //printf("hello\n");
	    intVectorType x0;
	    x0.x=x;
	    x0.y=y;
	    ACCESSDATAATPOINT(help,x,y) = dilateX(data,x0,range);
	  //printf("x=%d y=%d diletate=%f\n",x,y,value);
	};
    };
    for ( y=n+10;y<(*data).sizeY-(n+10);y++)
    {
	for ( x=n+10;x<(*data).sizeX-(n+10);x++)
	{
	  //printf("hello\n");
	    intVectorType x0;
	    x0.x=x;
	    x0.y=y;
	    ACCESSDATAATPOINT((*dilation),x,y) = dilateY(&help,x0,range);
	  //printf("x=%d y=%d diletate=%f\n",x,y,value);
	};
    };
    free(help.data);
    return 0;
};

float dilate (densityDataType* data, intVectorType x0, int range)
{
    float max,b;
    b=0;
    max=0;
    range=range/2;
    INTEGRATE(range, b=ACCESSDATAATPOINT((*data), x0.x-x, x0.y-y);if (b>max) max=b;);
    return max;
  // printf("max: %f\n", max);
}

int Dilation(densityDataType* data, int range, densityDataType* dilation)
{
    int x,y;
    int n=range/2;
    for ( y=n+10;y<(*data).sizeY-(n+10);y++)
    {
	for (x=n+10;x<(*data).sizeX-(n+10);x++)
	{
	    intVectorType x0;
	    x0.x=x;
	    x0.y=y;
	    ACCESSDATAATPOINT((*dilation),x,y) = dilate(data,x0,range);
	  //printf("x=%d y=%d diletate=%f\n",x,y,value);
	};
    };
  //printf("ready\n");
    return 0;
};


float erode (densityDataType* data,intVectorType x0, int range)
{
    float min,b;
    b=0;
    min=300;
    range=range/2;
    INTEGRATE(range,b=ACCESSDATAATPOINT((*data),x0.x-x, x0.y-y); if (b<min) min=b;);
    return min;
}

int Erosion(densityDataType* data, int range, densityDataType* erosion)
{
    int n=range/2;
    int x,y;
    for ( y=n+10;y<(*data).sizeY-(n+10);y++)
    {
	for ( x=n+10;x<(*data).sizeX-(n+10);x++)
	{
	    intVectorType x0;
	    x0.x=x;
	    x0.y=y;
	    ACCESSDATAATPOINT((*erosion), x, y) = erode(data,x0,range);
	  //printf("x=%d y=%d erode=%f\n",x,y,value);
	};
    };
     return 0;
};  

float erodeX (densityDataType* data, intVectorType x0, int range)
{
    float min,b;
    b=0;
    min=300;
    range=range/2;
    int x;
    for (x=-range; x<=+range; x=x+1)
    {
	b=ACCESSDATAATPOINT((*data), x0.x-x, x0.y);
	if (b<min) min=b;
    }
    return min;
}

float erodeY (densityDataType* data, intVectorType x0, int range)
{
    float min,b;
    b=0;
    min=300;
    range=range/2;
    int y;
    for ( y=-range; y<=+range; y=y+1)
    {
	b=ACCESSDATAATPOINT((*data), x0.x, x0.y-y);
	if (b<min) min=b;
    };
    return min;
}

int ErosionXY(densityDataType* data, int range, densityDataType* erosion)
{ 
    int height=(*data).sizeY;
    int width=(*data).sizeX;
    int imagesize=height*width+1;
    densityDataType help;
    (help).sizeX=width;
    (help).sizeY=height;
    (help).data=(float*) malloc( sizeof(float) * imagesize);
    int n=range/2;
    int x,y;
    for ( y=n+10;y<(*data).sizeY-(n+10);y++)
    {
	for ( x=n+10;x<(*data).sizeX-(n+10);x++)
	{
	  //printf("hello\n");
	    intVectorType x0;
	    x0.x=x;
	    x0.y=y;
	    ACCESSDATAATPOINT(help,x,y) = erodeX(data,x0,range);
	  //printf("x=%d y=%d diletate=%f\n",x,y,value);
	};
    };
    
    for ( y=n+10;y<(*data).sizeY-(n+10);y++)
    {
	for ( x=n+10;x<(*data).sizeX-(n+10);x++)
	{
	  //printf("hello\n");
	    intVectorType x0;
	    x0.x=x;
	    x0.y=y;
	    ACCESSDATAATPOINT((*erosion),x,y) = erodeY(&help,x0,range);
	  //printf("x=%d y=%d diletate=%f\n",x,y,value);
	};

    };

    free(help.data);
     return 0;
};

int Opening(densityDataType* data, int range, densityDataType* opening)
{
    int height=(*data).sizeY;
    int width=(*data).sizeX;
    int imagesize=height*width+1;
    densityDataType help;
    (help).sizeX=width;
    (help).sizeY=height;
    (help).data=(float*) malloc( sizeof(float) * imagesize);
    ErosionXY(data, range, &help);
    DilationXY(&help, range, opening);
    free(help.data);
    return 0;
}

int Closure(densityDataType* data, int range, densityDataType* closure)
{ 
    int height=(*data).sizeY;
    int width=(*data).sizeX;
    int imagesize=height*width+1;
    densityDataType help;
    (help).sizeX=width;
    (help).sizeY=height;
    (help).data=(float*) malloc( sizeof(float) * imagesize);
    DilationXY(data, range, &help);
    ErosionXY(&help, range, closure);
    free(help.data);
    return 0;
}

//Range of 9 is good
int Tophat(densityDataType* data, int range, densityDataType* tophat)
{
    float value;
    int n=range/2;
    int height=(*data).sizeY;
    int width=(*data).sizeX;
    int imagesize=height*width+1;
    densityDataType opening;
    (opening).sizeX=width;
    (opening).sizeY=height;
    (opening).data=(float*) malloc( sizeof(float) * imagesize);
    Opening(data, range, &opening); 
    int x,y;
    for ( y=n+10;y<height-(n+10);y++)
    {
	for ( x=n+10;x<width-(n+10);x++)
	{
	    value=ACCESSDATAATPOINT((*data),x,y)-ACCESSDATAATPOINT(opening, x, y);
	    ACCESSDATAATPOINT((*tophat), x, y)=value;
	};
    };
 free(opening.data);
 return 0;
}

//Range of 9 is good
int Tophat2(densityDataType* data, int range, densityDataType* tophat)
{
    float value;
    int n=range/2;
    int height=(*data).sizeY;
    int width=(*data).sizeX;
    int imagesize=height*width+1;
    densityDataType closure;
    (closure).sizeX=width;
    (closure).sizeY=height;
    (closure).data=(float*) malloc( sizeof(float) * imagesize);
    Closure(data, range, &closure);
    int x,y;
    for ( y=n+10;y<height-(n+10);y++)
    {
	for ( x=n+10;x<width-(n+10);x++)
	{
	    value=ACCESSDATAATPOINT((*data),x,y);
	    value=ACCESSDATAATPOINT(closure, x, y)-ACCESSDATAATPOINT((*data),x,y);
	    ACCESSDATAATPOINT((*tophat), x, y)=value;    
	};
    };

    free(closure.data);
    return 0;
}
