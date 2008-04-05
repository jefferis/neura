/*
 *  diffusion.c
 *  PolarAxonDensityTracker
 *
 *  Created by Philip J. Broser on Fri Jun 25 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "diffusion.h"
#include "moments.h"

#define EPS 0.000001
#include <math.h>

int MultiplyMatrix(floatMatrixType *result,floatMatrixType *A2,  floatMatrixType *A1)
{
//Multiply result=A2*A1
(*result).a11= (*A1).a11 * (*A2).a11 + (*A1).a21 * (*A2).a12; 
(*result).a12= (*A1).a12 * (*A2).a11 + (*A1).a22 * (*A2).a12; 

(*result).a21= (*A1).a11 * (*A2).a21 + (*A1).a21 * (*A2).a22; 
(*result).a22= (*A1).a12 * (*A2).a21 + (*A1).a22 * (*A2).a22; 

return 0;
}

int CopyMatrixTargetSource(floatMatrixType *target ,floatMatrixType *source)
{
(*target).a11=(*source).a11;
(*target).a12=(*source).a12;
(*target).a21=(*source).a21;
(*target).a22=(*source).a22;
return 0;
}



#define tau 0.4

float gradU_X(int x, int y,densityDataType *densityData)
{
//return ACCESSDATAATPOINT((*densityData), (x+1), y)-ACCESSDATAATPOINT((*densityData), x, y); //first order
//return 0.5*(ACCESSDATAATPOINT((*densityData), (x+1), y)-ACCESSDATAATPOINT((*densityData), (x-1), y)); //secondorder
	float up =   (ACCESSDATAATPOINT((*densityData), (x+1), y)-ACCESSDATAATPOINT((*densityData), (x)  , y));
	float low=   (ACCESSDATAATPOINT((*densityData), (x), y)  -ACCESSDATAATPOINT((*densityData), (x-1), y)); //upwindig
	return tau * up + (1.0 - tau ) * low;
};


float gradU_Y(int x, int y, densityDataType *densityData)
{
//return ACCESSDATAATPOINT((*densityData), x, (y+1))-ACCESSDATAATPOINT((*densityData), x, y); //first order
//return 0.5*(ACCESSDATAATPOINT((*densityData), x, (y+1))-ACCESSDATAATPOINT((*densityData), x, (y-1))); //second order
	float up = (ACCESSDATAATPOINT((*densityData), x, (y+1))-ACCESSDATAATPOINT((*densityData), x, (y)));
	float low= (ACCESSDATAATPOINT((*densityData), x, (y))  -ACCESSDATAATPOINT((*densityData), x, (y-1)));
	return 	tau * up + (1.0 - tau ) * low;
};


float DGradU_X(int x, int y, floatMatrixType *D,densityDataType *densityData)
{
	return (*D).a11*gradU_X(x,y,densityData)+(*D).a12*gradU_Y(x,y,densityData);
}
 
float DGradU_Y(int x, int y, floatMatrixType *D,densityDataType *densityData)
{
	return (*D).a21*gradU_X(x,y,densityData)+(*D).a22*gradU_Y(x,y,densityData);
}

float Divergence(int x, int y, floatMatrixType *D,densityDataType *densityData)
{
//	return DGradU_X(x+1,y,D,densityData)-DGradU_X(x,y,D,densityData)+DGradU_Y(x,y+1,D,densityData)-DGradU_Y(x,y,D,densityData); //first order

	return 0.5*(DGradU_X(x+1,y,D,densityData)-DGradU_X(x-1,y,D,densityData)+DGradU_Y(x,y+1,D,densityData)-DGradU_Y(x,y-1,D,densityData)); //second order

}




float divDGradU(intVectorType point ,densityDataType *densityData, int scanningRange)
{
	momentOfInertiaType Inertia;
	floatMatrixType BT, B, Diag, Help, Diffusion;
	float a, structure;
	//evaluateDirectionAndStructure(&a, &structure ,&Inertia, *densityData, point, scanningRange);
	
	/*
	(Inertia).v1.x=0.0;
	(Inertia).v1.y=1.0;
	(Inertia).v2.x=1.0;
	(Inertia).v2.y=0.0;
	*/
	
	//BTransposed
	float magV1=FLOATVECNORM((Inertia).v1)+EPS;
	float magV2=FLOATVECNORM((Inertia).v2)+EPS;
	
	/*
	printf("magV1=%f\n",magV1);
	printf("magV2=%f\n",magV2);
*/
	
	BT.a11=(Inertia).v1.x/magV1; BT.a12=(Inertia).v1.y/magV1; 
	BT.a21=(Inertia).v2.x/magV2; BT.a22=(Inertia).v2.y/magV2; 
	
	//Diagonal 
	Diag.a11=1.0; 		Diag.a12=0.0; 
	Diag.a21=0.0;		Diag.a22=1.0; 
	
	//B
	B.a11=(Inertia).v1.x/magV1; B.a12=(Inertia).v2.x/magV2; 
	B.a21=(Inertia).v1.y/magV1; B.a22=(Inertia).v2.y/magV2; 
	
	/*
	printf("BT:\n");
	SHOWMATRIX(BT);
	printf("Diag:\n");
	SHOWMATRIX(Diag); 
	*/
	
	//Create Diffusion Matrix
	MultiplyMatrix(&Help, &Diag, &BT); 
	
	/*
	printf("DiagBT:\n");
	SHOWMATRIX(Help); 
	*/
	/*
	printf("B:\n");
	SHOWMATRIX(B);
	 */
	 
	MultiplyMatrix(&Diffusion, &B, &Help); 
	
	
	//printf("Diffusion\n");
	//SHOWMATRIX(Diffusion); 
	

	return Divergence(point.x , point.y, &Diag , densityData);
	//return Divergence(point.x , point.y, &Diffusion , densityData);
};



int calcDiffusionTensorAtPoint(floatMatrixType *Tensor, int x, int y, densityDataType *densityData, int scanningRange)
{
/*
	//Diagonal
	//Debug 
	(*Tensor).a11=0.0; 		(*Tensor).a12=1.0; 
	(*Tensor).a21=1.0;		(*Tensor).a22=0.0;
	return 0;
*/

	intVectorType point;
	point.x=x;
	point.y=y; 
	momentOfInertiaType Inertia;
	floatMatrixType BT, B, Diag, Help;//, Diffusion;
	float a, structure;
	evaluateDirectionAndStructure(&a, &structure ,&Inertia, *densityData, point, scanningRange);
	
	//BTransposed
	float magV1=FLOATVECNORM((Inertia).v1)+EPS;
	float magV2=FLOATVECNORM((Inertia).v2)+EPS;
	
	BT.a11=(Inertia).v1.x/magV1; BT.a12=(Inertia).v1.y/magV1; 
	BT.a21=(Inertia).v2.x/magV2; BT.a22=(Inertia).v2.y/magV2; 
	
	//Diagonal 
	Diag.a11=1.0; 		Diag.a12=0.0; 
	Diag.a21=0.0;		Diag.a22=EPS; 
	
	//B
	B.a11=BT.a11; B.a12=BT.a21; 
	B.a21=BT.a12; B.a22=BT.a22; 
		
	//Create Diffusion Matrix
	MultiplyMatrix(&Help, &Diag, &BT); 
	MultiplyMatrix(Tensor, &B, &Help); 
	return 0;
}

float cD1(floatMatrixType *M) // calcultae D1
{
return sqrt( ((*M).a11* (*M).a11) + ((*M).a21* (*M).a21));
};

float cD2(floatMatrixType *M) //calcultae D2
{
return sqrt( ((*M).a22* (*M).a22) + ((*M).a12* (*M).a12));
};


#define U(x,y) ACCESSDATAATPOINT((*densityData), (x), (y))

float divDGradUAtPointFiniteVolume(intVectorType point ,densityDataType *densityData, int scanningRange)
{
	//Calcultae Tensors
	floatMatrixType T, TXp1, TXm1, TYp1, TYm1;
	int x=point.x;
	int y=point.y;
	calcDiffusionTensorAtPoint(&T, x,y, densityData, scanningRange);
	
	/*
	calcDiffusionTensorAtPoint(&TXp1, x+1,y, densityData, scanningRange);
	calcDiffusionTensorAtPoint(&TXm1, x-1,y, densityData, scanningRange);
	calcDiffusionTensorAtPoint(&TYp1, x,y+1, densityData, scanningRange);
	calcDiffusionTensorAtPoint(&TYm1, x,y-1, densityData, scanningRange);
	*/
	
	//Test!!!
	//Copy Matrix
	CopyMatrixTargetSource(&TXp1 ,&T);
	CopyMatrixTargetSource(&TXm1 ,&T);
	CopyMatrixTargetSource(&TYp1 ,&T);
	CopyMatrixTargetSource(&TYm1 ,&T);
	//Test!!!
	
	
	//Calculte D1
	float D1, D1Xp1, D1Xm1, D1Yp1, D1Ym1;
	D1=cD1(&T);
	//Abfangen das D1 0 ist, dann nur y assemblieren  vice versa auch mit D2 und y
	
	
	D1Xp1=cD1(&TXp1);
	D1Xm1=cD1(&TXm1);
	D1Yp1=cD1(&TYp1);
	D1Ym1=cD1(&TYm1);
	
	//Calculte D2
	float D2, D2Xp1, D2Xm1, D2Yp1, D2Ym1;
	D2=cD2(&T);
	D2Xp1=cD2(&TXp1);
	D2Xm1=cD2(&TXm1);
	D2Yp1=cD2(&TYp1);
	D2Ym1=cD2(&TYm1);
	
	//Calcultae U(x+1/2,y)=UXpH
	float UXpH, UXmH, UYpH, UYmH; // UX plus einhalb, UX minus einhalb
	
	UXpH=(D1* U(x,y) + D1Xp1* U(x+1,y) ) / (D1+D1Xp1);
	UXmH=(D1* U(x,y) + D1Xm1* U(x-1,y) ) / (D1+D1Xm1);
	
	UYpH=(D2* U(x,y) + D2Yp1* U(x,y+1) ) / (D2+D2Yp1);
	UYmH=(D2* U(x,y) + D2Ym1* U(x,y-1) ) / (D2+D2Ym1);
	
	//Caluclate Divergenz
	float divergence;
	divergence=2.0*(D1*(UXpH-2.0*U(x,y) + UXmH)+ D2 * (UYpH-2.0 * U(x,y) + UYmH));

	return divergence;

};

float min(float a, float b)
{
    if (a<b) return a;
    return b;

}

float max(float a, float b)
{
    if (a>b) return a;
    return b;
    
}


float divDGradUAtPointSevenPointStar(intVectorType point ,densityDataType *densityData, int scanningRange)
{
    //Calculte aij s
    floatMatrixType D;
    int x=point.x;
    int y=point.y;
    calcDiffusionTensorAtPoint(&D, x,y, densityData, scanningRange);
    
    //D.a11=1.0; 		D.a12=0.0; 
    //D.a21=0.0;		D.a22=1.0;
    
    float a12Minus=min(D.a12,0);
    float a12Plus=max(D.a12,0.0);
    float absa12=fabs(D.a12);
    
    float starUpperPart  =U(x-1,y-1)*(-1.0*a12Minus)+U(x,y-1)*(D.a22-absa12)+U(x+1,y-1)*a12Plus;
    float startMiddlePart=U(x-1,y)*(D.a11-absa12)+U(x,y)*2.0*(absa12-D.a11-D.a22)+U(x+1,y)*(D.a11-absa12);
    float startLowerPart =U(x-1,y+1)*a12Plus+U(x,y+1)*(D.a22-absa12)+U(x+1,y+1)*(-1.0*a12Minus);
    
    return starUpperPart+startMiddlePart+startLowerPart;
};

#undef U






int nextTatPoint(intVectorType point ,densityDataType *densityData, densityDataType *targetData, float deltaT, int scanningRange)
{
//float dU=divDGradU(point, densityData, scanningRange);
//float dU=divDGradUAtPointFiniteVolume(point, densityData, scanningRange);
    float dU=divDGradUAtPointSevenPointStar(point, densityData, scanningRange);

    ACCESSDATAATPOINT((*targetData), point.x, point.y)=ACCESSDATAATPOINT((*densityData), point.x, point.y)+deltaT*dU;
    return 0;
};

//Filtering
//Sensible Values
/*
int scanningRange=10;//7; 
	float deltaT=0.5;// max 1/4
	int maxTimeStep=4;
*/
int filterDensityData(densityDataType imageDensityData, densityDataType filteredData, int scanningRange, float deltaT, int maxTimeStep)
{
float percent;
 int timestep;
int y,x;
	printf("Start diffusion \n");
	for ( timestep=1;timestep<maxTimeStep+1;timestep=timestep+1)
	{
	for ( y=scanningRange+5;y<imageDensityData.sizeY-(scanningRange+5);y++)
	{
		int percent= (int) ((y*100.0)/(imageDensityData.sizeY-(scanningRange+5)));
		if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of time step %d of %d\n",percent, timestep, maxTimeStep);};
		for (x=scanningRange+5;x<imageDensityData.sizeX-(scanningRange+10);x++)
			{
			intVectorType point;
			point.x=x;
			point.y=y;
			nextTatPoint(point, &imageDensityData, &filteredData, deltaT, scanningRange);	
			};
			
		};		
	};

	return 0;
};






