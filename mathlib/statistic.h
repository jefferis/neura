/****************************************************************************/
/*                                                                          */
/* File:      statistic.h 	                                            */
/*                                                                          */
/* Purpose:   class for statistical manipulation of data                    */
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
/* History:   28.10.2002                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/


#ifndef STATISTIC_H
#define STATISTIC_H
#include <stdio.h>

#include "../datalib/volume.h"
#include "../datalib/vector.h"
#include "../datalib/datacube.h"

class Statistic 
{
 public:
  Statistic(DataCube *dCube);
  ~Statistic();

  double meanValue();
  double meanValue(Vector<int> &voxel, Vector<int> &size);
  
  
  double standardDeviation();
  double meanValue(int z);
  double standardDeviation(int z);
  
  double maxValue();
  double minValue();
  double maxValue(int z);
  double minValue(int z);


friend ostream &operator<<(ostream &ostr,Statistic &St);  
  
 private:
  DataCube *dataCube;
  
};

 
#endif

