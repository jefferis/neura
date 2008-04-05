/*
 *  moments.c
 *  PolarAxonDensityTracker
 *
 *  Created by Philip J. Broser on Fri Jun 25 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "moments.h"

#include <math.h>


#define DETERMINATE(A) A.a11*A.a22-A.a12*A.a21
#define CLEAR(A) A.a11=0.0;A.a12=0.0;A.a21=0.0;A.a22=0.0;

#define UpowerN(u) u*u*u

#define EPS 0.000001
#define DELTA 0.0001

#define INTEGRATE(POINT, RANGE, FUNCTION) \
int x,y; \
for (y=POINT.y-RANGE+1;y<POINT.y+RANGE;y=y+1) \
for (x=POINT.x-RANGE+1;x<POINT.x+RANGE;x=x+1) \
{ FUNCTION; }; 

//Auxillary
inline 
float determinate(floatMatrixType A)
{

    return A.a11*A.a22-A.a12*A.a21;
}

int sign (float x)
{
  return (x>=0)-(x<0);
  // return x>=0 ? 1:-1;
}

float Volume(densityDataType data,  intVectorType x0, int scanningRange)
{
	float v=0.0;
	INTEGRATE(x0, scanningRange, v=v+1.0;);
    return v;
}

floatMatrixType inverse(floatMatrixType A)
{
    floatMatrixType IA;
    float detA=determinate(A);
    if (fabs(detA)<EPS) 
    {
	
	IA.a11=1.0;  IA.a12=0.0;
	IA.a21=0.0;  IA.a22=1.0; 
	return IA;
    }
    IA.a11=A.a22/detA;		IA.a12=-1.0*A.a12/detA;
    IA.a21=-1.0*A.a21/detA;     IA.a22=IA.a11/detA; 
    return IA;
};

int eigenVectAndValues(float a, float b,float c,
		     float *lambda1, float *lambda2,
		     float *v1x, float *v1y, 
		     float *v2x, float *v2y)
{
	if (fabs(a)<EPS) a=EPS;
	if (fabs(b)<EPS) b=EPS;
	if (fabs(c)<EPS) c=EPS;
	


  double disc = sqrt((a-c)*(a-c)+4*b*b)/2;

  (*lambda1) = (a+c)/2 + disc;
  (*lambda2) = (a+c)/2 - disc;

  if (fabs((*lambda1)) < EPS && fabs((*lambda2)) < EPS)
    {
      //printf("\twarning: lambda1 = %f, lambda2 = %f\n",(*lambda1),(*lambda2));
      return 1;
    }

  (*v1x) = -b;  (*v1y) = a-(*lambda1);
  (*v2x) = -b;  (*v2y) = a-(*lambda2);
  float v1mag = sqrt((*v1x)*(*v1x) + (*v1y)*(*v1y))+EPS;  
  float v2mag = sqrt((*v2x)*(*v2x) + (*v2y)*(*v2y))+EPS;
  (*v1x)=(*v1x)/ v1mag;  (*v1y)=(*v1y)/v1mag;
  (*v2x)=(*v2x)/ v2mag;  (*v2y)=(*v2y)/v2mag;

  return 0;
}






//These Routines are unsecrure they assume that the integration area is all inside the data range!
float calculateMassArroundPoint(densityDataType data,  intVectorType x0, int scanningRange)
{
    
    float mass=0.0;
    
    INTEGRATE(x0, scanningRange, mass=mass+UpowerN(ACCESSDATAATPOINT(data, x, y)););
    
    return mass;
}


int calcualteCenterOfMassArroundPoint(floatVectorType *centerOfMass, densityDataType data,  intVectorType x0, int scanningRange, float *correctedMass)
{
    (*correctedMass)=calculateMassArroundPoint(data,  x0, scanningRange)+DELTA;
    (*centerOfMass).x=0.0;
    (*centerOfMass).y=0.0;
    float UpN; //stores image data U power N
    INTEGRATE(x0, scanningRange,
	      UpN=UpowerN(ACCESSDATAATPOINT(data, x, y));
	      (*centerOfMass).x=(*centerOfMass).x+UpN*(x-x0.x);
	      (*centerOfMass).y=(*centerOfMass).y+UpN*(y-x0.y);
	      );
    
    
    (*centerOfMass).x=(*centerOfMass).x/ (*correctedMass);
    (*centerOfMass).y=(*centerOfMass).y/ (*correctedMass);
    return 0;
}


int calculateTensorOfInertiaArroundPoint(floatMatrixType *Tensor, densityDataType data,  intVectorType x0, int scanningRange)
{
    CLEAR((*Tensor));
    
    
    float correctedMass;
    float helpX, helpY;
    float UpN; //stores image data U power N
    
    floatVectorType centerOfMass;
    calcualteCenterOfMassArroundPoint(&centerOfMass, data, x0, scanningRange, &correctedMass);
    
    INTEGRATE(x0, scanningRange,
	      helpX=(float) x-x0.x-centerOfMass.x;
	      helpY=(float) y-x0.y-centerOfMass.y;
		  UpN=UpowerN(ACCESSDATAATPOINT(data, x, y));      
	      (*Tensor).a11=(*Tensor).a11+UpN*helpX*helpX;  	(*Tensor).a12=(*Tensor).a12+UpN*helpX*helpY;
	      (*Tensor).a21=(*Tensor).a21+UpN*helpX*helpY;  	(*Tensor).a22=(*Tensor).a22+UpN*helpY*helpY;
	      );
    
    (*Tensor).a11=(*Tensor).a11/correctedMass;  (*Tensor).a12=(*Tensor).a12/correctedMass;
    (*Tensor).a21=(*Tensor).a21/correctedMass;  (*Tensor).a22=(*Tensor).a22/correctedMass;
    
    return 0;
}

  
int calculateMomentOfInertiaArroundPoint(momentOfInertiaType *Inertia, densityDataType data,  intVectorType point, int scanningRange)
{
	int lastError;
	//Calculate Tensor Matrix
    floatMatrixType Tensor;
    calculateTensorOfInertiaArroundPoint(&Tensor, data, point, scanningRange);
  	//TensorPlot
    //  	printf("a11: %f a12: %f\n",Tensor.a11, Tensor.a12);
    //	printf("a21: %f a22: %f\n",Tensor.a21, Tensor.a22);
      
	
  	//Eigen Values and Vectors
  	lastError=eigenVectAndValues(Tensor.a11, Tensor.a12,Tensor.a22,
		     &((*Inertia).alpha1), &((*Inertia).alpha2),
		     &((*Inertia).v1.x), &((*Inertia).v1.y), 
		     &((*Inertia).v2.x), &((*Inertia).v2.y));
  	
  	if (lastError) 
  	{
  	return 1; 
  	//printf("Error durring calcualtion of eigenvectors\n");  	
	}
	
	return 0;
};


int evaluateDirectionAndStructure(float *alpha, float *structure, momentOfInertiaType *Inertia, densityDataType data,  intVectorType point, int scanningRange)
{
	
	if (calculateMomentOfInertiaArroundPoint(Inertia,data,point,scanningRange))
	{
	//Moments could not be computed return equla eigenvalues 
	//Isotropic
	//v1
	(*Inertia).v1.x=1.0;
	(*Inertia).v1.y=0.0;
	(*Inertia).alpha1=0.5;
	//v2
	(*Inertia).v2.x=0.0;
	(*Inertia).v2.y=1.0;
	(*Inertia).alpha2=0.5;
	(*structure)=0.0;
	(*alpha)=0.0; //not defined !!!
	return 1;
	}; 
	//Sort Eigen Values and Vectors
	float helpX;
	float helpY;
	float helpAlpha;
	//Biggest Eigen Value should will be first eigenvalue 
	if ((*Inertia).alpha1<(*Inertia).alpha2)
	{
	//Store
	helpX=(*Inertia).v1.x;
	helpY=(*Inertia).v1.y;
	helpAlpha=(*Inertia).alpha1;
	//Switch
	//1
	(*Inertia).v1.x=(*Inertia).v2.x;
	(*Inertia).v1.y=(*Inertia).v2.y;
	(*Inertia).alpha1=(*Inertia).alpha2;
	//2
	(*Inertia).v2.x=helpX;
	(*Inertia).v2.y=helpY;
	(*Inertia).alpha2=helpAlpha;
	};
	//printf("v1.x:%f v1.y:%f alpha1:%f\n",Inertia.v1.x, Inertia.v1.y, Inertia.alpha1);
	//printf("v1.x:%f v1.y:%f alpha1:%f\n",(*Inertia).v1.x, (*Inertia).v1.y, (*Inertia).alpha1);
	
	//Calculate Structure Parameter
	(*structure)=((*Inertia).alpha1-(*Inertia).alpha2)/((*Inertia).alpha1+(*Inertia).alpha2+EPS);
	//Angle
	(*alpha)=acos((*Inertia).v1.y*sign((*Inertia).v1.x));
      	return 0;
	//(*alpha)=acos(-(*Inertia).v1.y/(sqrt((*Inertia).v1.x*(*Inertia).v1.x+(*Inertia).v1.y*(*Inertia).v1.y+EPS)));
	
	
	return 0;
}

int evaluateStructure(float *structure, densityDataType data,  intVectorType point, int scanningRange)
{
	float alpha;
	momentOfInertiaType Inertia;
	evaluateDirectionAndStructure(&alpha, structure, &Inertia, data, point, scanningRange);
	return 0;
}




/* int Segmentate(densityDataType axondata,int scanningRange) */
/* { */
/* float percent; */
 
/*  for (int y=scanningRange+10;y<data.sizeY-(scanningRange+10);y++) */
/*    { */
/*      int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+10))); */
/*      if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d\n",percent);}; */
/*      for (int x=scanningRange+10;x<data.sizeX-(scanningRange+10);x++) */
/*        { */
/* 	 //if (ACCESSDATAATPOINT(data, x, y)<0.1) */
/* 	 // { */
/* 	 //   structureCube(x,y,0)=0.0; */
/* 	 //  alphaCube(x,y,0)=0.0; */
/* 	 //  continue; */
/* 	 //} */
/* 	 intVectorType x0; */
/* 	 x0.x=x; */
/* 	 x0.y=y;  */
/* 	 momentOfInertiaType Inertia; */
/* 	 if (calculateMomentOfInertiaArroundPoint(&Inertia,data,point,scanningRange)) */
/* 	   { */
/* 	     //Moments could not be computed return equla eigenvalues  */
/* 	     //Isotropic */
/* 	     //v1 */
/* 	     Inertia.v1.x=1.0; */
/* 	     Inertia.v1.y=0.0; */
/* 	     Inertia.alpha1=0.5; */
/* 	     //v2 */
/* 	     Inertia.v2.x=0.0; */
/* 	     Inertia.v2.y=1.0; */
/* 	     Inertia.alpha2=0.5; */
/* 	     (*structure)=0.0; */
/* 	     (*alpha)=0.0; //not defined !!! */
/* 	     return 1; */
/* 	   };  */
/* 	 //Sort Eigen Values and Vectors */
/* 	 float helpX; */
/* 	 float helpY; */
/* 	 float helpAlpha; */
/* 	 //Biggest Eigen Value should will be first eigenvalue  */
/* 	 if (Inertia.alpha1<Inertia.alpha2) */
/* 	   { */
/* 	     //Store */
/* 	     helpX=Inertia.v1.x; */
/* 	     helpY=Inertia.v1.y; */
/* 	     helpAlpha=Inertia.alpha1; */
/* 	     //Switch */
/* 	     //1 */
/* 	     Inertia.v1.x=Inertia.v2.x; */
/* 	     Inertia.v1.y=Inertia.v2.y; */
/* 	     Inertia.alpha1=Inertia.alpha2; */
/* 	     //2 */
/* 	     Inertia.v2.x=helpX; */
/* 	     Inertia.v2.y=helpY; */
/* 	     Inertia.alpha2=helpAlpha; */
/* 	   } */
	 
/*        } */
/*    } */
/* } */


/* int findAxon(intVectorType point, densityDataType data, densityDataType axondata, int scanningRange) */
/* { */
/*   float percent; */
/*   float landa=1; */
/*   if (landa<0.2);// Abruchkriterium alternativ verhaeltnis alpha1 alpha2 */
/*     { */
/*       momentOfInertiaType Inertia; */
/*       if (calculateMomentOfInertiaArroundPoint(&Inertia,data,point,scanningRange)) */
/* 	{ */
/* 	  //Moments could not be computed return equla eigenvalues  */
/* 	  //Isotropic */
/* 	  //v1 */
/* 	  Inertia.v1.x=1.0; */
/* 	  Inertia.v1.y=0.0; */
/* 	  Inertia.alpha1=0.5; */
/* 	  //v2 */
/* 	  Inertia.v2.x=0.0; */
/* 	  Inertia.v2.y=1.0; */
/* 	  Inertia.alpha2=0.5; */
/* 	  return 1; */
/* 	};  */
     
/*       //Biggest Eigen Value should will be first eigenvalue  */
/*       if (Inertia.alpha1<Inertia.alpha2) */
/* 	{ */
/* 	  Inertia.v1.x=Inertia.v2.x; */
/* 	  Inertia.v1.y=Inertia.v2.y; */
/* 	  Inertia.alpha1=Inertia.alpha2; */
/* 	} */
/*       Inertia.v1.y=Inertia.v1.y*sign(Inertia.v1.x); */
/*       Inertia.v1.x=fabs(Inertia.v1.x); */
/*       landa=Inertia.alpha1; */
/*       point.x=landa*Inertia.v1.x; */
/*       point.y=landa*Inertia.v1.y; */
      
/*     } */
/* } */



