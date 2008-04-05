/*
 *  moments.h
 *  PolarAxonDensityTracker
 *
 *  Created by Philip J. Broser on Fri Jun 25 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include "datatypes.h"

//Auxillary
float determinate(floatMatrixType A);
floatMatrixType inverse(floatMatrixType A);


//Routines
float Volume(densityDataType data,  intVectorType x0, int scanningRange);
float calculateMassArroundPoint(densityDataType data,  intVectorType point, int scanningRange);
int calcualteCenterOfMassArroundPoint(floatVectorType *centerOfMass, densityDataType data,  intVectorType point, int scanningRange, float *correctedMass);
int calculateTensorOfInertiaArroundPoint(floatMatrixType *Tensor, densityDataType data,  intVectorType point, int scanningRange);
int calculateMomentOfInertiaArroundPoint(momentOfInertiaType *Inertia, densityDataType data,  intVectorType point, int scanningRange);
int evaluateDirectionAndStructure(float *alpha, float *structure,  momentOfInertiaType *Inertia, densityDataType data,  intVectorType point, int scanningRange);
int evaluateStructure(float *structure, densityDataType data,  intVectorType point, int scanningRange);







