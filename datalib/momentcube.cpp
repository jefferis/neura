#include "momentcube.h"


MomentCube:: 
MomentCube(int d) : Volume(d)
{
  dim=d;
  mass = 0;
  fp1 = 0;
  fp2 = 0;
  fp3 = 0;
  it11 = 0;
  it12 = 0;
  it13 = 0;
  it22 = 0;
  it23 = 0; 
  it33 = 0;
}


MomentCube:: 
~MomentCube()
{
 if ( allocated )
     {
       delete [] mass;
       delete [] fp1;
       delete [] fp2;
       delete [] fp3;
       delete [] it11;
       delete [] it12;
       delete [] it13;
       delete [] it22;
       delete [] it23;
       delete [] it33;

     }
}


int 
MomentCube:: 
MallocMomentCube()
{
 int fieldSize=1;
 int i;

 for (i=1;i<=dim;i++) fieldSize=fieldSize*size[i];

 mass = new(NeuraDataType[fieldSize]);
 fp1 = new(NeuraDataType[fieldSize]);
 fp2 = new(NeuraDataType[fieldSize]);
 fp3 = new(NeuraDataType[fieldSize]);
 it11 = new(NeuraDataType[fieldSize]);
 it12 = new(NeuraDataType[fieldSize]);
 it13 = new(NeuraDataType[fieldSize]);
 it22 = new(NeuraDataType[fieldSize]);
 it23 = new(NeuraDataType[fieldSize]);
 it33 = new(NeuraDataType[fieldSize]);

  if (
      (mass==NULL) ||
      (fp1==NULL) ||
      (fp2==NULL) ||
      (fp3==NULL) ||
      (it11==NULL) ||
      (it12==NULL) ||
      (it13==NULL) ||
      (it22==NULL) ||
      (it23==NULL) ||
      (it33==NULL)
      )
    {
      cout << "No Memory left! Quit!" << endl;
      return CANTMALLOC;
    }
  
  for ( i=0;i<fieldSize;i++) mass[i]=0.0;
  for ( i=0;i<fieldSize;i++) fp1[i]=0.0;
  for ( i=0;i<fieldSize;i++) fp2[i]=0.0;
  for ( i=0;i<fieldSize;i++) fp3[i]=0.0;
  for ( i=0;i<fieldSize;i++) it11[i]=0.0;
  for ( i=0;i<fieldSize;i++) it12[i]=0.0;
  for ( i=0;i<fieldSize;i++) it13[i]=0.0;
  for ( i=0;i<fieldSize;i++) it22[i]=0.0;
  for ( i=0;i<fieldSize;i++) it23[i]=0.0;
  for ( i=0;i<fieldSize;i++) it33[i]=0.0;

  allocated = 1;
  return OK;
}


NeuraDataType* 
MomentCube:: 
GetMassPointer()
{
  return mass;
}


NeuraDataType* 
MomentCube:: 
GetFP1Pointer()
{
  return fp1;
}


NeuraDataType* 
MomentCube:: 
GetFP2Pointer()
{
  return fp2;
}


NeuraDataType* 
MomentCube:: 
GetFP3Pointer()
{
  return fp3;
}


NeuraDataType* 
MomentCube:: 
GetIT11Pointer()
{
  return it11;
}


NeuraDataType* 
MomentCube:: 
GetIT12Pointer()
{
  return it12;
}


NeuraDataType* 
MomentCube:: 
GetIT13Pointer()
{
  return it13;
}


NeuraDataType* 
MomentCube:: 
GetIT22Pointer()
{
  return it22;
}


NeuraDataType* 
MomentCube:: 
GetIT23Pointer()
{
  return it23;
}


NeuraDataType* 
MomentCube:: 
GetIT33Pointer()
{
  return it33;
}







