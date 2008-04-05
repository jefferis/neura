#include "datatypes.h"

int MultiplyMatrix(floatMatrixType *result,floatMatrixType *A2,  floatMatrixType *A1);

float gradU_X(int x, int y,densityDataType *densityData);


float gradU_Y(int x, int y, densityDataType *densityData);

float DGradU_X(int x, int y, floatMatrixType *D,densityDataType *densityData);
 
float DGradU_Y(int x, int y, floatMatrixType *D,densityDataType *densityData);

float Divergence(int x, int y, floatMatrixType *D,densityDataType *densityData);

float divDGradU(intVectorType point ,densityDataType *densityData, int scanningRange);

float divDGradUAtPointFiniteVolume(intVectorType point ,densityDataType *densityData, int scanningRange);

float divDGradUAtPointSevenPointStar(intVectorType point ,densityDataType *densityData, int scanningRange);

int nextTatPoint(intVectorType point ,densityDataType *densityData, densityDataType *targetData, float deltaT, int scanningRange);



//Filtering
//Sensible Values
/*
int scanningRange=10;//7; 
	float deltaT=0.5;// max 1/4
	int maxTimeStep=4;
*/
int filterDensityData(densityDataType imageDensityData, densityDataType filteredData, int scanningRange, float deltaT, int maxTimeStep);


