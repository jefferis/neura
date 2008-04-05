/****************************************************************************/
/*                                                                          */
/* File:      datacube.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for data handling                            */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*            Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   27.09.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

#ifndef DATACUBE_H
#define DATACUBE_H
#include <stdio.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>

#include "volume.h"
#include "array.h"
#include "vector.h"
#include"../mathlib/multivector.h"
#include "../errorcode.h"

#define X_DIR 1
#define Y_DIR 2
#define Z_DIR 3

typedef float NeuraDataType; 

typedef unsigned char NeuraCompressionType;

class DataCube : public Volume
{ 
 public:

    //Initalize and destroy
    DataCube();
    DataCube(const int d);
    DataCube( DataCube &dc);
    ~DataCube();
    int MallocCube();
    void SetDataPointer(NeuraDataType *dP);  
    NeuraDataType* GetDataPointer();
    
    void freeMemory();
    int sleep();
    int wakeup();
    
  //Info
  int NrOfElements();
  friend ostream &operator<<(ostream &ostr, const DataCube &DC);
  NeuraDataType minValue();
  NeuraDataType maxValue();
  int CalcualteMinAndMax(NeuraDataType &min,NeuraDataType &max);
  double DCNorm(double x, double y, double z);
  void print();
  
  //Data manipulation
  void SetAll(NeuraDataType value);
  int operator-(DataCube& b);
  int operator=(DataCube& dc);

  
  
  //Dataaccess
  // needed for solver of christoph reisinger
  int CopyIntoMultiVector(MultiVector<double> &mv);
  int CopyFromMultiVector(MultiVector<double> &mv);
  int CopyIntoMultiVector(MultiVector<float> &mv);
  int CopyFromMultiVector(MultiVector<float> &mv);

  NeuraDataType &GetData(int k);
  NeuraDataType GetData(int x, int y, int z); // 3D acess with boundary control
  void SetData(int x, int y, int z, NeuraDataType val); // 3D acess with boundary control
  void SetData(int i, NeuraDataType val);
  NeuraDataType &operator ()(int k);  // direct data acess
  NeuraDataType &operator ()(int x, int y);         // 2D acess
  NeuraDataType &operator ()(int x, int y, int z);  // 3D acess with boundary control
  NeuraDataType &operator [](Vector<int> X);
  
  int CopyDataCubeIntoDataCube(DataCube *source,Vector<int> start, Vector<int> size, Vector<int> pos);

  //IO Functions
  int writePPM2D (char *filename);
  int writeSlicePPM (char *filename,int sliceNr, int direction);
  int writeSlicePPM (char *filename,int sliceNrStart,int sliceNrEnde,int direction);
  int writeSlicesPPM (char *filename, int direction);

  int writeRAW(char *name,char *info);
  int readRAW(char *name);

  int writeRAWZXSwitched(char *name,char *info);
  int writeToVrend(char* Name);
  
  int writeDataForDX(char *name);
  
  
  // 	reading and writing from and to xml.
  int ReadFromXML(char* strFileName);
  int WriteToXML(char* strFileName);

  //   reading and writing in nih Format
  int ReadFromNIH(char* name);
  int WriteToNIH(char* name);
  
  //    reading and writing from and to tiff
  int ReadFromTIFF(char* strFileName);
  int WriteToTIFF(char* strFileName);

  friend ostream &operator<<(ostream &ostr,DataCube &dC);
  
 protected:
  int flags; // Controll flags
  int allocated; // has data been allcated in constructor?
  NeuraDataType *data; //Pointer to data array, first position is 0
  bool sleeping;
  NeuraCompressionType *sleepingData;
  
};
 
#endif













