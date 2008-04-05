/*
 *  distancemap.h
 *  DistanceMap
 *
 *  Created by Philip J. Broser on Tue Jul 06 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DISTANCEMAP
#define DISTANCEMAP

#include "datatypes.h"

int createDistanceMapOfData(ucharDataType sourceData, densityDataType targetData);
int calcultateGaussianCurvature(densityDataType sourceData, densityDataType targetData);
int extractCenterLine(densityDataType sourceData, ucharDataType targetData);

float measureStructureLength(ucharDataType sourceData);
int executeSkeletonization(ucharDataType sourceData, ucharDataType targetData);

#endif