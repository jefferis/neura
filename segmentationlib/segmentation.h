/****************************************************************************/
/*                                                                          */
/* File:      segmentation.h 	                                            */
/*                                                                          */
/* Purpose:   class definition for interpolation data in datacubes          */
/*                                                                          */
/* Author:    Christine Vossen  and Philip Julian Broser                    */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     christine.vossen@gmx.de, philip.broser@iwr.uni-heidelberg.de  */
/*                                                                          */
/* History:   02.05.2003                                                    */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

/*
* The class segmentates the dataCube with help of the inbeded thresholding class, so that the constructor of
* the segmentation class needs as well parameters for the threshold class like sizeSmallCube.

* The function segmentateCube() compares the value of the threshold with the value of the source DataCube.
* And maps these values to 0 or 1 in the segmentatedDataCube, here called segmenDC.

* The function segmentateCubeByDevidingThroughTheThreshold() writes as values of the segmentated DataCube the 
* value of the source DataCube devided by the threshold.
*/

#ifndef SEGMENTATION_H
#define SEGMENTATION_H


#include <stdio.h>

#include "../datalib/vector.h"
#include "../datalib/volume.h"
#include "../mathlib/statistic.h"
#include "../errorcode.h"
#include "../datalib/datacube.h"
#include "interpolation.h"
#include "threshold.h"
#include "regiongrowing.h"
#include "point3d.h"




class Segmentation : public threshold 
{
  
 public:
  Segmentation(Vector<int> sizeSmallCube, DataCube *cube, int scalar, DataCube *segmenDC, double absMinTh, double absMaxTh, double minimalBetween2);
  Segmentation(DataCube *sourceCube, DataCube *targetCube, int (*callBack) (int percentage));
  Segmentation(DataCube *sourceCube, DataCube *targetCube);

  ~Segmentation();
  
  //possible kinds of segmentation 
  int SegmentateByOtsuMethod();
  int SegmentateByOtsuDevidingThroughTheThreshold();

  int SegmentateByStructuralThresholding(double abslow, double abshigh, double eps);
  int SegmentateByGlobalThresholding(double Threshold);

  int SegmentateByRegionGrowing(RegionGrowingTypes vMode, int Stoptime,int x, int y, int z );

 private:
  int (*callBackFunction) (int percentage);
  Statistic stat;
  DataCube *segDC;
  double absolutelowthreshold, absolutehighthreshold, epsilon;
  double CheckVoxel(int x, int y, int z);
};



#endif
