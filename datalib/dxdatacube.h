/****************************************************************************/
/*                                                                          */
/* File:      dxdatacube.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for conecting with openDX                    */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*                                                                          */
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


#ifndef DXDATACUBE_H
#define DXDATACUBE_H
#include <stdio.h>

#include "volume.h"
#include "vector.h"
#include "datacube.h"
#include "cfddatacube.h"
#include "../errorcode.h"
   
#define EXCHANGE_X_Z          1

class DXDataCube : public CfdDataCube
{
 public:
  DXDataCube();
  ~DXDataCube();
  int SetCubeToDX(int LposInKnt,int LpDim,int *LpCounts,float *Lorigin,float *Ldeltas,  int LinputCount,double *LinputData,int LoutputCount,double *LoutputData,int flags);
  void transferDXtoCube();
  void transferCubetoDX();

  friend ostream &operator<<(ostream &ostr, const DXDataCube &DC);  

 protected:
  int posInKnt;     //total Nr of Position
  int pDim;         //dimension of Position
  int *pCounts;     //nr of counts in each direction
  float *origin;    //origin of Grid
  float *deltas;    //deltas in each direction
  
  int inputCount;   //count of Inputdata
  double *inputData; //inputData

  int outputCount;  //count of output
  double *outputData;//outputData;
  
};
 
#endif













