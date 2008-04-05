
#include <math.h>
 
extern "C" 
{
#include "../toollib/moments.h"
#include "../toollib/diffusion.h"
#include "../toollib/segmentation.h"
}
#include "../datalib/datacube.h"
#include "../datalib/testcubes.h"

int mainTestSomeValueOfMomentOpperator(int argc, char **argv)      
{
	int lastErrorCode=0;
	densityDataType densityData;
	DataCube dataCube(3);
	lastErrorCode=dataCube.ReadFromTIFF("simple.tif"); // <---- Hier setzt man den File namen!!!!!!
	DataCube targetCube(dataCube);
	 
	if (lastErrorCode!=0 && lastErrorCode!=NO_SPACEINGS_TAGS)
	{
	printf("Error reding file!\n");
	return(1);
	}
	
	float value;
	int x;
	for(x=290;x<310;x++)
	{
	value=dataCube(x,100,0);
	printf("Value(%d,%d,1)=%f\n",x,100,value);
	}
	densityDataType data;
	data.sizeX=(dataCube.GetSize())[1];
    data.sizeY=(dataCube.GetSize())[2];
    data.data=dataCube.GetDataPointer();

	intVectorType point;
	point.x=300;
	point.y=100; 
	int scanningRange=10;
	momentOfInertiaType momentsAtPoint; 
	printf("Calc at Point: x:%d y:%d\n",point.x,point.y);
	calculateMomentOfInertiaArroundPoint(&momentsAtPoint, data,  point, scanningRange);
	//Output
	printf("Vector1:v1x:%f  v1y:%f alpha=%f\n",momentsAtPoint.v1.x,momentsAtPoint.v1.y,momentsAtPoint.alpha1);
	printf("Vector2:v2x:%f  v2y:%f alpha=%f\n",momentsAtPoint.v2.x,momentsAtPoint.v2.y,momentsAtPoint.alpha2);
	float alpha;
	float structure;
	momentOfInertiaType Inertia; 
	evaluateDirectionAndStructure(&alpha, &structure, &Inertia, data, point, scanningRange);
	printf("Alpha=%f in degrees\n",alpha*360.0/(2*3.1415926));
	printf("Structure=%f (0 is isotrop, 1 is linear) \n",structure); 
	
	if (dataCube.WriteToTIFF("testOut.tiff")) 
	{
	printf("Error durring write\n");
	return 1;
	};
	
	return 0;
	
} 


int mainA(int argc, char **argv)  //CreateStructureAndMomentCube    
{
	int lastErrorCode=0;
	densityDataType densityData;
	DataCube dataCube(3);
	//lastErrorCode=dataCube.ReadFromTIFF("testIn.tif"); // <---- Hier setzt man den File namen!!!!!!
	//lastErrorCode=dataCube.ReadFromTIFF("targetCube.tiff");
	//lastErrorCode=dataCube.ReadFromTIFF("ZProjectionOfMPI56.tif");∑
	//lastErrorCode=dataCube.ReadFromTIFF("0002.tif");
	lastErrorCode=dataCube.ReadFromTIFF("ZProjectionRBallMPI56.tif");
	dataCube.WriteToTIFF("loadedCube.tiff");
	//lastErrorCode=dataCube.ReadFromTIFF("simple.tif");
	//lastErrorCode=dataCube.ReadFromTIFF("dam_r5.tif");
	
	DataCube structureCube(dataCube);
	DataCube alphaCube(dataCube);
	  
	if (lastErrorCode!=0 && lastErrorCode!=NO_SPACEINGS_TAGS)
	{
	printf("Error reding file!\n");
	return(1);
	} 
	
	float value, structure, alpha; 
	int scanningRange=4;//7; 
	
	densityDataType data;
	data.sizeX=(dataCube.GetSize())[1];
    data.sizeY=(dataCube.GetSize())[2];
    data.data=dataCube.GetDataPointer();
	float percent;
	
	for (int y=scanningRange+10;y<data.sizeY-(scanningRange+10);y++)
	{
		int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+10)));
		if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d\n",percent);};
		for (int x=scanningRange+10;x<data.sizeX-(scanningRange+10);x++)
			{
			if (ACCESSDATAATPOINT(data, x, y)<0.1)
			{
				structureCube(x,y,0)=0.0;
				alphaCube(x,y,0)=0.0;
				continue;
			}
			intVectorType x0;
			x0.x=x;
			x0.y=y;
			momentOfInertiaType Inertia; 
			evaluateDirectionAndStructure(&alpha, &structure, &Inertia, data, x0, scanningRange);
			structureCube(x,y,0)=structure;
			alphaCube(x,y,0)=(alpha*360.0)/(2*3.14159265);
			//printf("x=%d y=%d Alpha=%f\n",x,y,(alpha*360.0)/(2*3.14159265));
			};
	};
	
	if (structureCube.WriteToTIFF("structureCube.tiff")) 
	{
	printf("Error durring write\n");
	return 1;
	};
	
	if (alphaCube.WriteToTIFF("alphaCube.tiff")) 
	{
	printf("Error durring write\n");
	return 1;
	};
	return 0;
	
}


int mainFilter(int argc, char **argv)  //ExecuteDiffusion      
{
	int lastErrorCode=0;
	DataCube dataCube(3);
	
	//lastErrorCode=dataCube.ReadFromTIFF("testIn2.tif"); // <---- Hier setzt man den File namen!!!!!!
	//lastErrorCode=dataCube.ReadFromTIFF("simple.tif");
	lastErrorCode=dataCube.ReadFromTIFF("dam_r5.tif");
	DataCube targetCube(dataCube);
	  
	if (lastErrorCode!=0 && lastErrorCode!=NO_SPACEINGS_TAGS)
	{
	printf("Error reding file!\n");
	return(1);
	} 
	
	float value, structure, alpha; 
	int scanningRange=4; 
	float deltaT=0.25;// max 1/4
	int maxTimeStep=4;
	
	densityDataType target, data;
	data.sizeX=(dataCube.GetSize())[1];
    data.sizeY=(dataCube.GetSize())[2];
    data.data=dataCube.GetDataPointer();
	target.sizeX=(dataCube.GetSize())[1];
    target.sizeY=(dataCube.GetSize())[2];
    target.data=targetCube.GetDataPointer();

	intVectorType x0;
	x0.x=100;
	x0.y=100;

	printf("Volume of scanning range: %d = %f\n",scanningRange, Volume(data, x0, scanningRange));
	
	float percent;
	printf("Start diffusion \n");
	for (int timestep=1;timestep<maxTimeStep+1;timestep=timestep+1)
	{
	for (int y=scanningRange+5;y<data.sizeY-(scanningRange+5);y++)
	{
		int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+5)));
		if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of time step %d of %d\n",percent, timestep, maxTimeStep);};
		for (int x=scanningRange+5;x<data.sizeX-(scanningRange+10);x++)
			{
			intVectorType point;
			point.x=x;
			point.y=y;
			nextTatPoint(point, &data, &target, deltaT, scanningRange);

			
			};
			
			
	};
	
	//Copy Data
	dataCube=targetCube;
	
	};
	
	if (targetCube.WriteToTIFF("TargetCube.tiff")) 
	{
	printf("Error durring write\n");
	return 1;
	};
	
	scanningRange=5;
	//Segmentation
	for (int y=scanningRange+5;y<data.sizeY-(scanningRange+5);y++)
	{
		int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+5)));
		if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of segmentation\n",percent);};
		for (int x=scanningRange+5;x<data.sizeX-(scanningRange+10);x++)
			{
			intVectorType point;
			point.x=x;
			point.y=y;
			structuralThresholdingAtPoint(point ,&data, &target, 0.1, 0.01, 0.7, scanningRange);
			};
	};
	
	
	

	if (targetCube.WriteToTIFF("segTargetCube.tiff")) 
	{
	printf("Error durring write\n");
	return 1;
	};
	
	
	return 0;
	
}



int main(int argc, char **argv)    //TestTheOpperator
{
	floatMatrixType BT, B, Diag, Help, Diffusion;
	BT.a11=3; BT.a12=44; 
	BT.a21=33; BT.a22=322; 

	B.a11=49; B.a12=34; 
	B.a21=13; B.a22=32; 

	MultiplyMatrix(&Diffusion, &B, &BT); 
	printf("a11: %f a12: %f\n",Diffusion.a11, Diffusion.a12);
  	printf("a21: %f a22: %f\n",Diffusion.a21, Diffusion.a22);

	//TestOpps
	densityDataType target, data;
	DataCube dataCube(3);
	int lastErrorCode=dataCube.ReadFromTIFF("testIn2.tif");
	DataCube targetCube(dataCube);
	data.sizeX=(dataCube.GetSize())[1];
    data.sizeY=(dataCube.GetSize())[2];
    data.data=dataCube.GetDataPointer();
	target.sizeX=(dataCube.GetSize())[1];
    target.sizeY=(dataCube.GetSize())[2];
    target.data=targetCube.GetDataPointer();

	float value, structure, alpha; 
	int scanningRange=4; 
	float deltaT=.5;
	int maxTimeStep=1;
	
	for (int y=0;y<data.sizeY;y++)
	{
		for (int x=0;x<data.sizeX;x++)
			dataCube(x,y,0)=x*x+y*y;  
	}
	
	floatMatrixType D;
	D.a11=1.0; 		D.a12=0.0; 
	D.a21=0.0;		D.a22=1.0; 
	
	
	for (int y=scanningRange+5;y<data.sizeY-(scanningRange+5);y++)
	{
	 for (int x=scanningRange+5;x<data.sizeX-(scanningRange+10);x++)
			{
			intVectorType point;
			point.x=x;
			point.y=y;
			//float lap=divDGradUAtPointFiniteVolume(point ,&data, scanningRange);
			float lap=divDGradUAtPointSevenPointStar(point ,&data, scanningRange);

			printf("x:%d y:%d U:%f laplace:%f\n",x,y,ACCESSDATAATPOINT(data, x, y), lap);
			
			/*
			printf("x:%d y:%d U:%f gradUX:%f gradUY:%f\n",x,y,ACCESSDATAATPOINT(data, x, y) ,gradU_X( x, y,&data),gradU_Y( x, y,&data));
			printf("DGradU_x:%f\n",DGradU_X(x, y, &D,&data));
 			printf("DGradU_y:%f\n",DGradU_Y(x, y, &D,&data));
 			*/
 			
 			//printf("Divergence:%f\n", Divergence(x, y, &D, &data));
			
			
			
			//nextTatPoint(point, &data, &target, deltaT, scanningRange);

			
			}};




	return 0;

}


int mainS(int argc, char **argv)  //ExecuteDiffusion      
{
	int lastErrorCode=0;
	DataCube dataCube(3);
	//lastErrorCode=dataCube.ReadFromTIFF("targetCube.tiff");
	lastErrorCode=dataCube.ReadFromTIFF("ZProjectionOfMPI56.tif");
	DataCube targetCube(dataCube);
	  
	if (lastErrorCode!=0 && lastErrorCode!=NO_SPACEINGS_TAGS)
	{
	printf("Error reding file!\n");
	return(1);
	} 
	
	float value, structure, alpha; 
	int scanningRange=4; 
	float deltaT=0.25;// max 1/4
	int maxTimeStep=4;
	
	densityDataType target, data;
	data.sizeX=(dataCube.GetSize())[1];
    data.sizeY=(dataCube.GetSize())[2];
    data.data=dataCube.GetDataPointer();
	target.sizeX=(dataCube.GetSize())[1];
    target.sizeY=(dataCube.GetSize())[2];
    target.data=targetCube.GetDataPointer();
	float percent;
	
	scanningRange=7;
	//Segmentation
	for (int y=scanningRange+5;y<data.sizeY-(scanningRange+5);y++)
	{
		int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+5)));
		if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of segmentation\n",percent);};
		for (int x=scanningRange+5;x<data.sizeX-(scanningRange+10);x++)
			{
			intVectorType point;
			point.x=x;
			point.y=y;
			//structuralThresholdingAtPoint(point ,&data, &target, 0.1, 0.11, 0.6, scanningRange);
			float structuresignal=0.17;
			float absMin=0.01;
			float minEps=0.0;
			float absMax=0.6;
			momentThresholdingAtPoint(point ,&data, &target, structuresignal,absMin, minEps, absMax, scanningRange);
			
			
			};
	};
	
	
	

	if (targetCube.WriteToTIFF("segTargetCube.tiff")) 
	{
	printf("Error durring write\n");
	return 1;
	};
	
	
	return 0;
	
}











