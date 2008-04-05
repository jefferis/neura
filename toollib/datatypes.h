/*
 *  datatypes.h
 *  PolarAxonDensityTracker
 *
 *  Created by Philip J. Broser on Fri Jun 25 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef DATATYPES
#define DATATYPES

struct intVectorStruct
{
    int x;
    int y;
};



struct floatVectorStruct
{
    float x;
    float y;
};

struct floatMatrixStruct
{
    float a11,  a12;
    float a21,  a22;
};

struct densityDataStruct
{
    int sizeX;
    int sizeY;
    float *data;
};

struct ucharDataStruct
{
    int sizeX;
    int sizeY;
    unsigned char *data;
};




typedef struct intVectorStruct intVectorType;
typedef struct floatVectorStruct floatVectorType;
#define FLOATVECNORM(v) sqrt(v.x*v.x+v.y*v.y)
 
typedef struct densityDataStruct densityDataType;


#define ACCESSDATAATPOINT(dataType, x, y)  dataType.data[(x)+(y)*dataType.sizeX]

typedef struct floatMatrixStruct floatMatrixType;

#define SHOWMATRIX(A) printf("a11: %f a12: %f\na21: %f a22: %f\n",A.a11, A.a12, A.a21, A.a22);
  	


struct momentOfInertiaStruct
{
    floatVectorType v1;
    float alpha1;
    floatVectorType v2;
    float alpha2;
};

typedef struct momentOfInertiaStruct momentOfInertiaType;

typedef struct ucharDataStruct ucharDataType;

void CreateBinaryData(ucharDataType *data,int sizeX, int sizeY);
void DestroyBinaryData(ucharDataType *data);
unsigned char GetBinaryDataAtPoint(ucharDataType data,int x, int y);
void SetBinaryDataAtPoint(ucharDataType data,int x, int y, unsigned char d);
int writeBinaryDataToPPMFile(char *fileName, ucharDataType data);



#endif

