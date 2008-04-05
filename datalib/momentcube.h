/****************************************************************************/
/*                                                                          */
/* File:      momentcube.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for store moments                            */
/*            all over the datacube                                         */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   25.03.03 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/


#ifndef MOMENTCUBE_H
#define MOMENTCUBE_H

#include <stdio.h>
#include <fstream.h>

#include "datacube.h"

class MomentCube : public Volume
{

 public:
  
  MomentCube(int d);
  ~MomentCube();

  int MallocMomentCube();

  NeuraDataType* GetMassPointer();

  NeuraDataType* GetFP1Pointer();
  NeuraDataType* GetFP2Pointer();
  NeuraDataType* GetFP3Pointer();

  NeuraDataType* GetIT11Pointer();
  NeuraDataType* GetIT12Pointer();
  NeuraDataType* GetIT13Pointer();
  NeuraDataType* GetIT22Pointer();
  NeuraDataType* GetIT23Pointer();
  NeuraDataType* GetIT33Pointer();


 private:

  int allocated;
  
  NeuraDataType* mass;
  
  NeuraDataType* fp1;
  NeuraDataType* fp2;
  NeuraDataType* fp3;
  
  NeuraDataType* it11; 
  NeuraDataType* it12; 
  NeuraDataType* it13;
  NeuraDataType* it22;
  NeuraDataType* it23;
  NeuraDataType* it33;

};

#endif
