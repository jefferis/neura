/*
 *  distancemap.c
 *  DistanceMap
 *
 *  Created by Philip J. Broser on Tue Jul 06 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "distancemap.h"
#define REALYBIG 10000


void setMinOf9Neigbourhood(int x, int y, densityDataType targetData)
{
    
    float presentDistance=ACCESSDATAATPOINT(targetData, x, y);
    float deltaDist=0.0;
    float minNeigbour=presentDistance;

//4neigbourhood
		//top
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x, y-1)+1))
    {
	deltaDist=1;
	minNeigbour=ACCESSDATAATPOINT(targetData, x, y-1);
	presentDistance=deltaDist+minNeigbour;
    };
		//bottom
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x, y+1)+1))
    {
	deltaDist=1;
	minNeigbour=ACCESSDATAATPOINT(targetData, x, y+1);
	presentDistance=deltaDist+minNeigbour;
    };
		//right
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x+1, y)+1))
    {
	deltaDist=1;
	minNeigbour=ACCESSDATAATPOINT(targetData, x+1, y);
	presentDistance=deltaDist+minNeigbour;
    };
		//left
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x-1, y)+1))
    {
	deltaDist=1;
	minNeigbour=ACCESSDATAATPOINT(targetData, x-1, y);
	presentDistance=deltaDist+minNeigbour;
    };
    
  
		//topright
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x+1, y-1)+1.4142))
    {
	deltaDist=1.4142;
	minNeigbour=ACCESSDATAATPOINT(targetData, x+1, y-1);
	presentDistance=deltaDist+minNeigbour;
    }; 
		//bottompright
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x+1, y+1)+1.4142))
    {
	deltaDist=1.4142;
	minNeigbour=ACCESSDATAATPOINT(targetData, x+1, y+1);
	presentDistance=deltaDist+minNeigbour;
    };  
	    //bottompleft
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x-1, y+1)+1.4142))
    {
	deltaDist=1.4142;
	minNeigbour=ACCESSDATAATPOINT(targetData, x-1, y+1);
	presentDistance=deltaDist+minNeigbour;
    };  
    
		 //topleft
    if (presentDistance>(ACCESSDATAATPOINT(targetData, x-1, y-1)+1.4142))
    {
	deltaDist=1.4142;
	minNeigbour=ACCESSDATAATPOINT(targetData, x-1, y-1);
	presentDistance=deltaDist+minNeigbour;
    };  
   
    
    ACCESSDATAATPOINT(targetData, x, y)=presentDistance;
    float result=ACCESSDATAATPOINT(targetData, x, y);
    
    return;
}


int createDistanceMapOfData(ucharDataType sourceData, densityDataType targetData)
{


    int x,y; 
    int sizeX=sourceData.sizeX;
    int sizeY=sourceData.sizeY;
    
    //Set Object Distance big and backrgoud distance to zero
    for (y=0;y<sizeY;y=y+1) 
	for (x=0;x<sizeX;x=x+1) 
	{ 
	    if (GetBinaryDataAtPoint(sourceData, x, y))
		
	    {
		if (x<2 | y<2 | x> sizeX-2 | y>sizeY-2) continue;
		ACCESSDATAATPOINT(targetData, x, y)=REALYBIG;
	    }
	    else
	    {
		ACCESSDATAATPOINT(targetData, x, y)=0.0;
	    };
	}; 
    
   // return 0; //debug
    
    //Start from the top and calculate min distances for objects
    for (y=1;y<(sizeY-1);y=y+1) 
	for (x=1;x<(sizeX-1);x=x+1) 
	   if (GetBinaryDataAtPoint(sourceData, x, y))
	   	setMinOf9Neigbourhood( x, y, targetData);		
    
    //Start from the top and calculate min distances for objects
    for (y=1;y<(sizeY-1);y=y+1) 
	for (x=(sizeX-2);x>0;x=x-1) 
	    if (GetBinaryDataAtPoint(sourceData, x, y))
	   	setMinOf9Neigbourhood( x, y, targetData);		
    
    
    
    
    //Start from the bottom and calculate min distances for objects
    for (y=(sizeY-2);y>0;y=y-1) 
	for (x=1;x<(sizeX-1);x=x+1) 
	   if (GetBinaryDataAtPoint(sourceData, x, y))
	   	setMinOf9Neigbourhood( x, y, targetData);
    
    
    //Start from the top and calculate min distances for objects
    for (y=(sizeY-2);y>0;y=y-1)
	for (x=(sizeX-2);x>0;x=x-1) 
	    if (GetBinaryDataAtPoint(sourceData, x, y))
	   	setMinOf9Neigbourhood( x, y, targetData);		
    



    return 0;
}



int IsBiggestOrEqual(densityDataType sourceData, int x, int y)
{
    float myData=ACCESSDATAATPOINT(sourceData, x, y);
    int dx, dy;
    
    /*
    for (dy=-1;dy<2;dy++)
	for (dx=-1;dx<2;dx++)
	    if(ACCESSDATAATPOINT(sourceData, (dx+x), (dy+y))>myData) return 0;
   */
    if(ACCESSDATAATPOINT(sourceData, (x+1), (y+0))>myData) return 0;
    if(ACCESSDATAATPOINT(sourceData, (x+0), (y+1))>myData) return 0;
    if(ACCESSDATAATPOINT(sourceData, (x-1), (y+0))>myData) return 0;
    if(ACCESSDATAATPOINT(sourceData, (x+0), (y-1))>myData) return 0;
    return 1;
}



float DDataDX(densityDataType sourceData, int x, int y)
{
    return (ACCESSDATAATPOINT(sourceData, (x+1), (y))-ACCESSDATAATPOINT(sourceData, (x), (y)));
}

float DDataDY(densityDataType sourceData, int x, int y)
{
    return (ACCESSDATAATPOINT(sourceData, (x), (y+1))-ACCESSDATAATPOINT(sourceData, (x), (y)));
}


float D2DataDX2(densityDataType sourceData, int x, int y)
{
    return (ACCESSDATAATPOINT(sourceData, (x+1), (y))+ACCESSDATAATPOINT(sourceData, (x-1), (y)) - 2.0*ACCESSDATAATPOINT(sourceData, (x), (y)));
}

float D2DataDY2(densityDataType sourceData, int x, int y)
{
    return ACCESSDATAATPOINT(sourceData, (x), (y+1))+ACCESSDATAATPOINT(sourceData, (x), (y-1)) - 2.0*ACCESSDATAATPOINT(sourceData, (x), (y));
}

float D2DataDXY(densityDataType sourceData, int x, int y)
{
    return ACCESSDATAATPOINT(sourceData, (x+1), (y+1))-ACCESSDATAATPOINT(sourceData, (x), (y+1))-ACCESSDATAATPOINT(sourceData, (x+1), (y))+ACCESSDATAATPOINT(sourceData, (x), (y));
																	       
}



float gaussianCurvature(densityDataType sourceData, int x, int y)
{
    float rhoXX=D2DataDX2(sourceData, x, y);
    float rhoYY=D2DataDY2(sourceData, x, y);
    float rhoXY=D2DataDXY(sourceData, x, y);
    float rhoX=DDataDX(sourceData, x, y);
    float rhoY=DDataDY(sourceData, x, y);

    float divisor=(1.0+rhoX*rhoX+rhoY*rhoY);
    divisor=divisor*divisor;
    return fabs((rhoXX*rhoYY-rhoXY*rhoXY)/(1.0+rhoX*rhoX+rhoY*rhoY));

}


int calcultateGaussianCurvature(densityDataType sourceData, densityDataType targetData)
{
    int x,y;
    int sizeX=sourceData.sizeX;
    int sizeY=sourceData.sizeY;
    
    for (y=0;y<(sizeY);y=y+1) 
	for (x=0;x<(sizeX);x=x+1) 
	    ACCESSDATAATPOINT(targetData, x, y)=0.0;
    
    
    
    for (y=2;y<(sizeY-2);y=y+1) 
	for (x=2;x<(sizeX-2);x=x+1) 
	    ACCESSDATAATPOINT(targetData, x, y)=gaussianCurvature(sourceData, x, y);
    
    return 0;
}



int extractCenterLine(densityDataType sourceData, ucharDataType targetData)
{
    int x,y;
    int sizeX=sourceData.sizeX;
    int sizeY=sourceData.sizeY;
    
    for (y=1;y<(sizeY-1);y=y+1) 
	for (x=1;x<(sizeX-1);x=x+1) 
	    if (ACCESSDATAATPOINT(sourceData, x, y)>0.0)
		
	    {
		if (IsBiggestOrEqual(sourceData, x,y))
		{
		    SetBinaryDataAtPoint(targetData, x, y,1);
		    continue;
		}
		else
		{
		    
		    SetBinaryDataAtPoint(targetData, x, y,0);
		    continue;
		}
	    }
		
		else
		{
		    SetBinaryDataAtPoint(targetData, x, y,0);
		};
    
    
    
    return 0;
}


int PixelIsDeletable(densityDataType sourceData, int x, int y)
{




    return 0;

}



float legthOfNeigbourhood(ucharDataType sourceData, int posX, int posY)
{
    float sum=0.0;
    if (GetBinaryDataAtPoint(sourceData, posX+1, posY+0))
    {
	sum=sum+1;
    };
    
    if (GetBinaryDataAtPoint(sourceData, posX+0, posY+1))
    {
	sum=sum+1;
    };
    
    if (GetBinaryDataAtPoint(sourceData, posX+1, posY+1))
    {
	sum=sum+1.4142;
    };
	
    return sum;
};



float measureStructureLength(ucharDataType sourceData)
{

    int x,y; 
    int sizeX=sourceData.sizeX;
    int sizeY=sourceData.sizeY;
    float sumLength=0.0;
    
    //Set Object Distance big and backrgoud distance to zero
    for (y=1;y<(sizeY-1);y=y+1) 
	for (x=1;x<(sizeX-1);x=x+1) 
	{ 
	    if (GetBinaryDataAtPoint(sourceData, x, y))
		
	    {
		sumLength=sumLength+legthOfNeigbourhood(sourceData, x, y);
	    }
	    else
	    {
		//do nothing
	    };
	}; 

    return sumLength;

}










//This part has been lend by:
/*---------------------------------------------------------------------------*/
/* Program:  skeleton.c                                                      */
/*                                                                           */
/* Purpose:  This program uses an exact Euclidean distance transform         */
/*           to calculate the skeleton of a binary image.  The -i flag       */
/*           inverts the B/W image before calculating the skeleton.          */
/*           The -t flag removes all skeleton points whose involutes         */
/*           (points which are on opposite side of the skeleton) are         */
/*           less than a specified distance apart.  The radius of the        */
/*           skeleton is output when the -r flag is used.                    */
/*                                                                           */
/* Author:   John Gauch                                                      */
/*                                                                           */
/* Date:     June 13, 1996                                                   */
/*                                                                           */
/* Note:     Copyright (C) The University of Kansas, 1996                    */
/*---------------------------------------------------------------------------*/

/* Neighborhood size constants */
#define K1   1
#define K2   169		/* really 13*13 */
#define K3   961		/* really 31*31 */
#define K4   2401		/* really 49*49 */
#define K5   5184		/* really 72*72 */

#ifndef MAXFLOAT
#define MAXFLOAT 1000000.0
#endif

/* Boolean values */
#define TRUE 1
#define FALSE 0
#define VALID 1
#define INVALID 0

/* Pixel format codes */
#define MINTYPE 1
#define BYTE 1
#define SHORT 2
#define INT 3
#define FLOAT 4
#define DOUBLE 5
#define COMPLEX 6
#define COLOR 7
#define PSEUDO 8
#define JPEG_GRAY 9
#define JPEG_RGB 10
#define MAXTYPE 10

/* Pixel format types */
typedef unsigned char BYTE_TYPE;
typedef short SHORT_TYPE;
typedef int INT_TYPE;
typedef float FLOAT_TYPE;
typedef double DOUBLE_TYPE;


/* Global variables */
SHORT_TYPE **Data1;
SHORT_TYPE **Xnear;
SHORT_TYPE **Ynear;
FLOAT_TYPE **Dist;
int Xdim, Ydim, Zdim;
int Debug = FALSE;
int Radius = FALSE;
int Invert = FALSE;
float MinDist = 10;
double sqrt();

/*---------------------------------------------------------------------------*/
/* Purpose:  This macro checks the specified neighbor.                       */
/*---------------------------------------------------------------------------*/
#define CHECK(dx,dy)\
{\
    x2=x+(dx);\
	y2=y+(dy);\
	    if ((x2>=0) && (y2>=0) && (x2<=Xdim-1) && (y2<=Ydim-1))\
		if (Dist[y2][x2] < Dist[y][x])\
		{\
		    Dx = Xnear[y2][x2] - x;\
			Dy = Ynear[y2][x2] - y;\
			    NewDist = Dx*Dx + Dy*Dy;\
				if (NewDist < Dist[y][x])\
				{\
				    Xnear[y][x] = Xnear[y2][x2];\
					Ynear[y][x] = Ynear[y2][x2];\
					    Dist[y][x] = (float)NewDist;\
				}\
		}\
}

/*---------------------------------------------------------------------------*/
/* Purpose:  This routine calculates the skeleton of an image.               */
/*---------------------------------------------------------------------------*/
void Skeleton()
{
    int x, y, x2, y2, Dx, Dy, NewDist, MaxDist;
    
    /* Invert the image */
    if (Invert == TRUE)
	for (y = 0; y < Ydim; y++)
	    for (x = 0; x < Xdim; x++)
	    {
		if (Data1[y][x] <= 0)
		    Data1[y][x] = 1;
		else
		    Data1[y][x] = 0;
	    }
		
		/* Initialize output image */
		if (Debug == TRUE)
		    printf("Initialize\n");
    for (y = 0; y < Ydim; y++)
	for (x = 0; x < Xdim; x++)
	{
	    if (Data1[y][x] == 1)
	    {
		Xnear[y][x] = x;
		Ynear[y][x] = y;
		Dist[y][x] = 0;
	    }
	    else
	    {
		Xnear[y][x] = 0;
		Ynear[y][x] = 0;
		Dist[y][x] = (float) (Xdim * Xdim + Ydim * Ydim);
	    }
	}
	    
	    /* Forward X, forward Y pass */
	    if (Debug == TRUE)
		printf("Forward X, forward Y pass\n");
    for (y = 0; y < Ydim; y++)
	for (x = 0; x < Xdim; x++)
	{
	    if (Dist[y][x] > K1)
	    {
		CHECK(-1, 0);
		CHECK(-1, -1);
		CHECK(0, -1);
	    }
	    if (Dist[y][x] > K2)
	    {
		CHECK(-2, -1);
		CHECK(-1, -2);
	    }
	    if (Dist[y][x] > K3)
	    {
		CHECK(-3, -1);
		CHECK(-3, -2);
		CHECK(-2, -3);
		CHECK(-1, -3);
	    }
	    if (Dist[y][x] > K4)
	    {
		CHECK(-4, -1);
		CHECK(-4, -3);
		CHECK(-3, -4);
		CHECK(-1, -4);
	    }
	    if (Dist[y][x] > K5)
	    {
		CHECK(-5, -1);
		CHECK(-5, -2);
		CHECK(-5, -3);
		CHECK(-5, -4);
		CHECK(-1, -5);
		CHECK(-2, -5);
		CHECK(-3, -5);
		CHECK(-4, -5);
	    }
	}
	    
	    /* Backward X, forward Y pass */
	    if (Debug == TRUE)
		printf("Backward X, forward Y pass\n");
    for (y = 0; y < Ydim; y++)
	for (x = (Xdim - 1); x >= 0; x--)
	{
	    if (Dist[y][x] > K1)
	    {
		CHECK(1, 0);
		CHECK(1, -1);
		CHECK(0, -1);
	    }
	    if (Dist[y][x] > K2)
	    {
		CHECK(2, -1);
		CHECK(1, -2);
	    }
	    if (Dist[y][x] > K3)
	    {
		CHECK(3, -1);
		CHECK(3, -2);
		CHECK(2, -3);
		CHECK(1, -3);
	    }
	    if (Dist[y][x] > K4)
	    {
		CHECK(4, -1);
		CHECK(4, -3);
		CHECK(3, -4);
		CHECK(1, -4);
	    }
	    if (Dist[y][x] > K5)
	    {
		CHECK(5, -1);
		CHECK(5, -2);
		CHECK(5, -3);
		CHECK(5, -4);
		CHECK(1, -5);
		CHECK(2, -5);
		CHECK(3, -5);
		CHECK(4, -5);
	    }
	}
	    
	    /* Forward X, backward Y pass */
	    if (Debug == TRUE)
		printf("Forward X, backward Y pass\n");
    for (y = (Ydim - 1); y >= 0; y--)
	for (x = 0; x < Xdim; x++)
	{
	    if (Dist[y][x] > K1)
	    {
		CHECK(-1, 0);
		CHECK(-1, 1);
		CHECK(0, 1);
	    }
	    if (Dist[y][x] > K2)
	    {
		CHECK(-2, 1);
		CHECK(-1, 2);
	    }
	    if (Dist[y][x] > K3)
	    {
		CHECK(-3, 1);
		CHECK(-3, 2);
		CHECK(-2, 3);
		CHECK(-1, 3);
	    }
	    if (Dist[y][x] > K4)
	    {
		CHECK(-4, 1);
		CHECK(-4, 3);
		CHECK(-3, 4);
		CHECK(-1, 4);
	    }
	    if (Dist[y][x] > K5)
	    {
		CHECK(-5, 1);
		CHECK(-5, 2);
		CHECK(-5, 3);
		CHECK(-5, 4);
		CHECK(-1, 5);
		CHECK(-2, 5);
		CHECK(-3, 5);
		CHECK(-4, 5);
	    }
	}
	    
	    /* Backward X, backward Y pass */
	    if (Debug == TRUE)
		printf("Backward X, backward Y pass\n");
    for (y = (Ydim - 1); y >= 0; y--)
	for (x = (Xdim - 1); x >= 0; x--)
	{
	    if (Dist[y][x] > K1)
	    {
		CHECK(1, 0);
		CHECK(1, 1);
		CHECK(0, 1);
	    }
	    if (Dist[y][x] > K2)
	    {
		CHECK(2, 1);
		CHECK(1, 2);
	    }
	    if (Dist[y][x] > K3)
	    {
		CHECK(3, 1);
		CHECK(3, 2);
		CHECK(2, 3);
		CHECK(1, 3);
	    }
	    if (Dist[y][x] > K4)
	    {
		CHECK(4, 1);
		CHECK(4, 3);
		CHECK(3, 4);
		CHECK(1, 4);
	    }
	    if (Dist[y][x] > K5)
	    {
		CHECK(5, 1);
		CHECK(5, 2);
		CHECK(5, 3);
		CHECK(5, 4);
		CHECK(1, 5);
		CHECK(2, 5);
		CHECK(3, 5);
		CHECK(4, 5);
	    }
	}
	    
	    /* Find the skeleton */
	    if (Debug == TRUE)
		printf("Find skeleton\n");
    for (y = 0; y < Ydim; y++)
	for (x = 0; x < Xdim; x++)
	{
	    MaxDist = 0;
	    if (y > 0)
	    {
		Dx = Xnear[y][x] - Xnear[y - 1][x];
		Dy = Ynear[y][x] - Ynear[y - 1][x];
		NewDist = (int) sqrt((double) (Dx * Dx + Dy * Dy));
		if (NewDist > MaxDist)
		    MaxDist = NewDist;
	    }
	    if (x > 0)
	    {
		Dx = Xnear[y][x] - Xnear[y][x - 1];
		Dy = Ynear[y][x] - Ynear[y][x - 1];
		NewDist = (int) sqrt((double) (Dx * Dx + Dy * Dy));
		if (NewDist > MaxDist)
		    MaxDist = NewDist;
	    }
	    /* JJJ
		if ((x > 0) && (y > 0))
	    {
		    Dx = Xnear[y][x] - Xnear[y-1][x-1];
		    Dy = Ynear[y][x] - Ynear[y-1][x-1];
		    NewDist = (float)sqrt((double)(Dx*Dx + Dy*Dy));
		    if (NewDist > MaxDist) MaxDist = NewDist;
	    }
	    
	    if ((x < Xdim-1) && (y > 0))
	    {
		Dx = Xnear[y][x] - Xnear[y-1][x+1];
		Dy = Ynear[y][x] - Ynear[y-1][x+1];
		NewDist = (float)sqrt((double)(Dx*Dx + Dy*Dy));
		if (NewDist > MaxDist) MaxDist = NewDist;
	    }
	    JJJ */
	    
	    /* Output the skeleton */
	    if (MaxDist < MinDist)
		Dist[y][x] = 0;
	    else if (Radius == TRUE)
		Dist[y][x] = (float) sqrt((double) (Dist[y][x]));
	    else
		Dist[y][x] = 1;
	}
}




#define Error(s) {printf(s);exit(1);};
//#define NULL 0


char *alloc1D(int Ydim, int Xdim, int Size)
{
    int i, Count;
    char *Data;

    /* Allocate data buffer */
    Count = Xdim * Ydim;
    if ((Data = (char *) malloc((unsigned) (Count * Size))) == NULL)
	Error("Could not allocate image buffer");
    for (i = 0; i < Count * Size; i++)
	Data[i] = 0;
    
    return (Data);
}



char **alloc2D(int Ydim, int Xdim, int Size)
{
    char *Data1, **Data2;
    int y, Count, Offset;
    
       
    /* Allocate row access buffer */
    Count = Ydim;
    if ((Data2 = (char **) malloc((unsigned) (Count * sizeof (char *)))) == NULL)
	Error("Could not allocate row access buffer");
    
    /* Initialize row access buffer */
    Offset = Xdim * Size;
    
    Data1 = alloc1D(Ydim, Xdim, Size);
    
    for (y = 0; y < Count; y++)
	Data2[y] = Data1 + y * Offset;
    return (Data2);
}

void free2D(unsigned char ** Data)
{
    free(Data[0]);
    free(Data);
}



int executeSkeletonization(ucharDataType sourceData, ucharDataType targetData)
{
    Xdim=sourceData.sizeX; 
    Ydim=sourceData.sizeY;
    

    /* Prepare Global variables */
    Data1= (SHORT_TYPE **) alloc2D( Ydim, Xdim, sizeof(SHORT_TYPE));
    Xnear= (SHORT_TYPE **) alloc2D( Ydim, Xdim, sizeof(SHORT_TYPE));
    Ynear= (SHORT_TYPE **) alloc2D( Ydim, Xdim, sizeof(SHORT_TYPE));
    Dist=  (FLOAT_TYPE **) alloc2D( Ydim, Xdim, sizeof(FLOAT_TYPE));;
    
    int x,y;
    for (y=0;y<Ydim;y++)
	for (x=0;x<Xdim;x++)
	{
	 if(GetBinaryDataAtPoint(sourceData,x,y))
	 {
	     Data1[y][x]=0.0;
	 }
	    else
	    {
	    Data1[y][x]=1.0;
	    }
	 
	 
	 };
    //Execute
    Skeleton();
    
    
    //Copy Result to final data
    for (y=0;y<Ydim;y++)
	for (x=0;x<Xdim;x++)
	    if(Dist[y][x]==1.0)
		SetBinaryDataAtPoint(targetData,x,y,1);
		
    
    //Free data structs
    free2D( (unsigned char**) Data1);
    free2D( (unsigned char**) Xnear);
    free2D( (unsigned char**) Ynear);
    free2D( (unsigned char**) Dist);
	   
    return 0;
}







