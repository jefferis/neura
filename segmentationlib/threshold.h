/****************************************************************************/
/*                                                                          */
/* File:      threshold.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for interpolation data in datacubes          */
/*                                                                          */
/* Author:    Christine Vossen                                              */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     christine.vossen@gmx.de                                       */
/*                                                                          */
/* History:   02.05.2003                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

// This class is constructed to calculate the local thresholds of the given DataCube. Local means that the programm devides the whole 
// cube into small ones (therefore is the parameter sizeSmallCube) which overlap and calculates the threshold for the middle of each 
// small cube.
// The threshold is calculated after the clustering method taken from the paper
//    "Lecture 4: Thresholding" of
//    Bryan S. Morse, Brigham Young University, 1998-2000

// Afterwards the other values (the points which are not middle of a small cube) are interpolated by the class interpolation.
// After inicialising the class the public function "lokalThreshold()" manages the described process.
// with the function "getOtsuCube()" a pointer of the DataCube with the calculated thresholds is returned.
 
 


#ifndef THRESHOLD_H
#define THRESHOLD_H


#include <stdio.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../errorcode.h"
#include "../datalib/datacube.h"
#include "interpolation.h"


class threshold {

 private:
  int (*callBackFunction) (int percentage);
   
   //Calculation: mean of the field below ( m[0]) the threshold T and abow ( m[1] )
  int calculateMean(double *mean, int *nFeld, double *pFeld, int T);
  
  //Calculation of local fields: nField: number of pixel belonging to one colour
  //                             pField: probability distribution of one colour 
  int calculateFields(int *nFeld, double *pFeld, Vector<int> middle);

  //Calculation of the sum of probabilities below n[0] and abow n[1] the threshold T
  int calculateN(double *pFeld, double *n,int T);

  //calculation of the between-class which has to be maximized for the threshold
  int calculateBetween(double *n, double *mean,int T, double &between2);

  //calculation of the threshold - old -running- version (slow)
  int oldVersion(int *nFeld,double *pFeld, int &T);

  //calculation of the threshold - optimated Version
  int optVersion(int *nFeld,double *pFeld, int &T);

  // calculation of the threshold of small cubes - respects marginal problems 
  int calculateLokalThreshold(Vector<int> middleSmallCube, int &T);
  
  
 protected:
  DataCube *targetOtsuCube;                             // threshold DataCube claculated with the Otsu Method
  DataCube *sourceDC;                                         // source DataCube
  Vector<int> sizeSC, size, start;
  double minBetween2, MinThreshold,  MaxThreshold;
  int k;
  
 public:
  threshold(Vector<int> sizeSmallCube, DataCube *cube, int scalar, double absMinTh, double absMaxTh, double minimalBetween2);
  threshold(DataCube *sourceCube);
  threshold(DataCube *sourceCube,int (*callBack) (int percentage)); 
~threshold();
  
  DataCube* getOtsuCube();
  int calcualteLokalOtsuThreshold();
 
};



#endif
