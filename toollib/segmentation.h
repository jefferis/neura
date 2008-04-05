

#ifndef SEGMENTATION
#define SEGMENTATION

#include "datatypes.h"
#include <stdio.h>


//Pointwise Thresholding
int structuralThresholdingAtPoint(intVectorType point ,densityDataType *sourceData, densityDataType *targetData, float absMin, float deltaEpsilon, float absMax,int scanningRange);
int momentThresholdingAtPoint(intVectorType point ,densityDataType *sourceData, densityDataType *targetData, float structuresignal, float absMin, float deltaEpsilon, float absMax,int scanningRange);




//Full Thresholding
int globalThresholding (densityDataType data, ucharDataType segData, float value);
int structuralThresholding (densityDataType data, densityDataType target);

int OtsuThresholding(densityDataType data, ucharDataType target);
int OtsuThresholdingWithParameter(densityDataType data, ucharDataType target, int globalMinThreshold, int globalMaxThreshold, int gridSize);
int structuralThresholdingWithParameter(densityDataType data, ucharDataType target, float globalMinThreshold, float globalMaxThreshold, float epsilon, int scanningRange);


#endif




 